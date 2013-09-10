{-# LANGUAGE ViewPatterns #-}

{-# OPTIONS_GHC -Wall -Werror #-}
module Main where

import Control.Applicative hiding (empty)
import Control.Monad.State.Strict
import Control.Monad.Trans.Maybe

import Data.Char
import Data.Foldable
import Data.Monoid
import Data.Sequence

import Numeric.Natural

import System.Environment
import System.IO

import Text.Parsec hiding ((<|>), many)
import Text.Parsec.String

import Prelude hiding (length, replicate)

data FueueItem = FPlus | FMinus | FMult | FDiv | FInv | FDup | FSwap | FPop | FCopy | FBlockify | FAppend | FDeblock | FHalt | FBlock (Seq FueueItem) | FNum Integer | FCache (Seq FueueItem) deriving (Eq)

type Fueue = StateT Natural (MaybeT IO)

main :: IO ()
main = do
  hSetBuffering stdout NoBuffering
  args <- getArgs
  interp <- getProgName
  (filename, program) <- case args of
    ["-e", p] -> return ("command line", p)
    [f] -> (,) f <$> readFile f
    [] -> (,) "stdin" <$> getContents
    _ -> error $ unlines [
      "",
      "Usage: ",
      interp ++ " <filename",
      interp ++ " -e <program text>",
      interp,
      "Note: No argument gives the program as input. No further input can then",
      "be given to the program."]
  case parseFueue filename program of
    Left err -> print err
    Right fueue -> finalRun fueue
      

mushCaches :: Seq FueueItem -> Seq FueueItem
mushCaches (viewr -> (viewr -> queue :> FCache cache1) :> FCache cache2) = queue |> FCache (cache1 >< cache2)
mushCaches queue = queue

inCache :: Seq FueueItem -> FueueItem -> Seq FueueItem
inCache (viewr -> queue :> FCache cache) item = queue |> FCache (cache |> item)
inCache queue item = queue |> FCache (singleton item)


step :: Seq FueueItem -> Fueue (Seq FueueItem)
step (viewl -> EmptyL) = error "Taneb's done something stupid!"
step (viewQueueNum -> Just (FPlus, arg1, arg2, rest)) = rest |> FNum (arg1 + arg2) <$ put 0
step (viewQueue2 -> Just (FMinus, FNum arg1, rest)) = rest |> FNum (negate arg1) <$ put 0
step (viewQueueNum -> Just (FMult, arg1, arg2, rest)) = rest |> FNum (arg1 * arg2) <$ put 0
step (viewQueueNum -> Just (FDiv, arg1, arg2, rest)) = rest |> FNum (arg1 `div` arg2) <$ put 0
step (viewQueue2 -> Just (FInv, FNum 0, rest)) = rest |> FNum 1 <$ put 0
step (viewQueue2 -> Just (FInv, FNum _, rest)) = rest |> FNum 0 <$ put 0
step (viewQueue2 -> Just (FDup, arg1, rest)) = rest |> arg1 |> arg1 <$ put 0
step (viewQueue3 -> Just (FSwap, arg1, arg2, rest)) = rest |> arg2 |> arg1 <$ put 0
step (viewQueue2 -> Just (FPop, _, rest)) = rest |> FCache empty <$ put 0
step (viewQueue3 -> Just (FCopy, FNum arg1, arg2, rest)) = (rest >< replicate (fromInteger arg1) arg2) |> FCache empty <$ put 0
step (viewQueue2 -> Just (FBlockify, arg1, rest)) = rest |> FBlock (singleton arg1) <$ put 0
step (viewQueue3 -> Just (FAppend, FBlock arg1, arg2, rest)) = rest |> FBlock (arg1 |> arg2) <$ put 0
step (viewQueue2 -> Just (FDeblock, FBlock arg1, rest)) = (rest >< arg1) |> FCache empty <$ put 0
step (viewl -> FHalt :< _) = mzero
step (viewl -> FNum arg1 :< rest) = rest |> FCache empty <$ (put 0 *> liftIO (putChar . chr $ fromInteger arg1))
step (viewl -> FCache (viewr -> EmptyR) :< queue) = pure queue
step (viewl -> FCache (viewr -> (viewr -> newCache :> FAppend) :> FBlock block) :< queue) = (FAppend <| FBlock block <| queue) |> FCache newCache <$ modify (+ fromIntegral (length newCache))
step (viewl -> FCache (viewr -> (viewr -> newCache :> FDeblock) :> FBlock block) :< queue) = (FDeblock <| FBlock block <| queue) |> FCache newCache <$ modify (+ fromIntegral (length newCache))
step (viewl -> FCache (viewr -> newCache :> arg1) :< queue) = arg1 <| mushCaches (queue |> FCache newCache) <$ modify (+ fromIntegral (length newCache))
step (viewl -> arg1 :< queue) = queue `inCache` arg1 <$ modify (+ 1)
step _ = error "Taneb's done something foolish!"


viewQueue1 :: Seq FueueItem -> Maybe (FueueItem, Seq FueueItem)
viewQueue1 (viewl -> EmptyL) = Nothing
viewQueue1 (viewl -> FCache (viewl -> EmptyL) :< rest) = viewQueue1 rest
viewQueue1 (viewl -> FCache (viewl -> a :< newCache) :< rest) = Just (a, FCache newCache <| rest)
viewQueue1 (viewl -> a :< rest) = Just (a, rest)
viewQueue1 _ = error "Taneb's done something idiotic!"

viewQueue2 :: Seq FueueItem -> Maybe (FueueItem, FueueItem, Seq FueueItem)
viewQueue2 queue = do
  (a, queue') <- viewQueue1 queue
  (b, rest) <- viewQueue1 queue'
  return (a, b, rest)

viewQueue3 :: Seq FueueItem -> Maybe (FueueItem, FueueItem, FueueItem, Seq FueueItem)
viewQueue3 queue = do
  (a, b, queue') <- viewQueue2 queue
  (c, rest) <- viewQueue1 queue'
  return (a, b, c, rest)

viewQueueNum :: Seq FueueItem -> Maybe (FueueItem, Integer, Integer, Seq FueueItem)
viewQueueNum queue = do
  (op, queue') <- viewQueue1 queue
  (FNum a, FNum b, rest) <- viewQueue2 queue'
  return (op, a, b, rest)

lengthOfFueue :: Seq FueueItem -> Natural
lengthOfFueue = fromInteger . getSum . foldMap (Sum . fueueItemLength)
  where fueueItemLength (FCache cache) = fromIntegral (length cache)
        fueueItemLength _ = 1
                                  
check :: Seq FueueItem -> Fueue (Seq FueueItem)
check prog = do
  time <- get
  if time >= lengthOfFueue prog
     then do
          put 0
          c <- toInteger . ord <$> liftIO getChar
          return $ prog |> FNum c
     else return prog

bigStep :: Seq FueueItem -> Fueue (Seq FueueItem)
bigStep = check >=> step

run :: Seq FueueItem -> Fueue a
run = bigStep >=> run

finalRun :: Seq FueueItem -> IO ()
finalRun prog = () <$ runMaybeT (evalStateT (run prog) 0)

parseFueue :: SourceName -> String -> Either ParseError (Seq FueueItem)
parseFueue = parse parseFueue1

parseFueue1 :: Parser (Seq FueueItem)
parseFueue1 = fromList <$> manyTill parseFueueItem (try $ spaces *> eof)

parseBlock :: Parser FueueItem
parseBlock = FBlock . fromList <$> manyTill parseFueueItem (try $ spaces *> char ']')

parseFueueItem :: Parser FueueItem
parseFueueItem = spaces *> (
  FPlus <$ char '+' <|>
  FMinus <$ char '-' <|>
  FMult <$ char '*' <|>
  FDiv <$ char '/' <|>
  FInv <$ char '%' <|>
  FDup <$ char ':' <|>
  FSwap <$ char '~' <|>
  FPop <$ char '!' <|>
  FCopy <$ char '$' <|>
  FBlockify <$ char '(' <|>
  FAppend <$ char '<' <|>
  FDeblock <$ char ')' <|>
  FHalt <$ char 'H' <|>
  FNum . read <$> many1 digit <|>
  char '[' *> parseBlock
  )
                 
debugRender :: FueueItem -> String
debugRender FPlus = "+"
debugRender FMinus = "-"
debugRender FMult = "*"
debugRender FDiv = "/"
debugRender FInv = "%"
debugRender FDup = ":"
debugRender FPop = "!"
debugRender FSwap = "~"
debugRender FCopy = "$"
debugRender FBlockify = "("
debugRender FAppend = "<"
debugRender FDeblock = ")"
debugRender FHalt = "H"
debugRender (FNum n) = ' ' : show n
debugRender (FBlock block) = '[' : foldMap debugRender block ++ "]"
debugRender (FCache cache) = '{' : foldMap debugRender cache ++ "}"

instance Show FueueItem where
  show (FCache cache) = foldMap debugRender cache
  show anythingElse = debugRender anythingElse