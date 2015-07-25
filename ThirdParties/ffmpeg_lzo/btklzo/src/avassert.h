/**
 * This is a modified file to keep only the macro av_assert0
 * The modified version of the macro throw a standard c++ exception
 *
 * The license for this file is the same than the one used by the FFmpeg project
 */

#ifndef AVUTIL_AVASSERT_H
#define AVUTIL_AVASSERT_H

#include <stdexcept>

#define av_assert0(cond) do {             \
    if (!(cond)) {                        \
        std::runtime_error("AV_ASSERT0"); \
    }                                     \
} while (0)

#endif /* AVUTIL_AVASSERT_H */
