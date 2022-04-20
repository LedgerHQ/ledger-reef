/*******************************************************************************
*   (c) 2018 Zondax GmbH
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
********************************************************************************/
#pragma once

#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-nullptr"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#pragma ide diagnostic ignored "modernize-deprecated-headers"

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include "string.h"

#ifndef __APPLE__
extern void explicit_bzero(void *s, size_t n) __THROW __nonnull ((1));
#endif

#define __Z_INLINE inline __attribute__((always_inline)) static
#define NV_ALIGN __attribute__ ((aligned(64)))

#if defined(LEDGER_SPECIFIC)
#include "bolos_target.h"
#endif

#if defined (TARGET_NANOS) || defined(TARGET_NANOX) || defined(TARGET_NANOS2)
#include "zxmacros_ledger.h"
#else
#include "zxmacros_x64.h"
#endif

#ifndef UNUSED
#define UNUSED(x) (void)x
#endif

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define ZX_SWAP(v) (((v) & 0x000000FFu) << 24u | ((v) & 0x0000FF00u) << 8u | ((v) & 0x00FF0000u) >> 8u | ((v) & 0xFF000000u) >> 24u)
#define HtoNL(v) ZX_SWAP( v )
#define NtoHL(v) ZX_SWAP( v )
#else
#define HtoNL(x) (x)
#define NtoHL(x) (x)
#endif

#define SET_NV(DST, TYPE, VAL) { \
    TYPE nvset_tmp=(VAL); \
    MEMCPY_NV((void*) PIC(DST), (void *) PIC(&nvset_tmp), sizeof(TYPE)); \
}

__Z_INLINE void strncpy_s(char *dst, const char *src, size_t dstSize) {
    MEMZERO(dst, dstSize);
    strncpy(dst, src, dstSize - 1);
}

#define sizeof_field(type, member) sizeof(((type *)0)->member)
#define array_length(array) (sizeof(array) / sizeof((array)[0]))

#define ZEMU_TRACE_EX(FORMAT, ...) { \
    char trace_buffer[50];              \
    snprintf(trace_buffer, sizeof(trace_buffer), FORMAT, __VA_ARGS__); \
    zemu_log_stack(trace_buffer);                          \
}

#define ZEMU_TRACE() /*ZEMU_TRACE_EX("%s [%d]\n", __FILE__,  __LINE__)*/

void check_app_canary();
void handle_stack_overflow();
void zemu_log_stack(const char *ctx);

#if defined(ZEMU_LOGGING)
__Z_INLINE void zemu_log(const char *buf)
{
    #if defined (TARGET_NANOS) || defined(TARGET_NANOX) || defined(TARGET_NANOS2)
    asm volatile (
    "movs r0, #0x04\n"
    "movs r1, %0\n"
    "svc      0xab\n"
    :: "r"(buf) : "r0", "r1"
    );
    #endif
}
#else
__Z_INLINE void zemu_log(const char *unused){
    (void)unused;
}
#endif

#ifdef __cplusplus
}
#endif

#pragma clang diagnostic pop

#include "zxformat.h"
