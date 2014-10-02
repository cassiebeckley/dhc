#ifndef DHC_KERNEL_MAYBE_HPP
#define DHC_KERNEL_MAYBE_HPP

namespace dhc {
    namespace kernel {

        /**
         * Represents a value which either contains a value or is empty
         */
        template <class T>
        class Maybe {
            public:
                /**
                 * \brief Create a Maybe with a value
                 *
                 * @param v The value of the Maybe
                 */
                Maybe(T v) : data(v), valued(true) {}

                /**
                 * \brief Create an empty Maybe
                 */
                Maybe() : valued(false) {}

                /**
                 * @return false if empty, true if not
                 */
                operator bool() const
                {
                    return valued;
                }

                const T data;

            protected:
            private:
                const bool valued;
        };

    }
}

#endif
