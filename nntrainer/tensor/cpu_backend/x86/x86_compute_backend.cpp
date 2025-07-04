// SPDX-License-Identifier: Apache-2.0
/**
 * Copyright (C) 2024 Sungsik Kong <ss.kong@samsung.com>
 *
 * @file   x86_compute_backend.cpp
 * @date   23 April 2024
 * @see    https://github.com/nnstreamer/nntrainer
 * @author Sungsik Kong <ss.kong@samsung.com>
 * @bug    No known bugs except for NYI items
 * @brief  Compute backend for x86
 *
 */

#include <assert.h>

#include <avx2_impl.h>
#include <cblas_interface.h>
#include <fallback_internal.h>
#include <nntrainer_error.h>
#include <x86_compute_backend.h>
#ifdef ENABLE_GGML
#include <ggml_interface.h>
#endif

#define ROW_MAJOR 0
#define COL_MAJOR 1

namespace nntrainer {

void init_backend() {
#ifdef ENABLE_GGML
  __ggml_init();
#else
  // TODO it needed.
#endif
}

void scopy_int4_to_float32(const unsigned int N, const uint8_t *X,
                           const unsigned int incX, float *Y,
                           const unsigned int incY) {
  __fallback_scopy_int4_to_float32(N, X, incX, Y, incY);
}

void copy_s16(const unsigned int N, const int16_t *X, int16_t *Y) {
  __fallback_copy_s16(N, X, Y);
}

void copy_u16(const unsigned int N, const uint16_t *X, uint16_t *Y) {
  __fallback_copy_u16(N, X, Y);
}

void copy_s16_fp32(const unsigned int N, const int16_t *X, float *Y) {
  __fallback_copy_s16_fp32(N, X, Y);
}

void copy_u16_fp32(const unsigned int N, const uint16_t *X, float *Y) {
  __fallback_copy_u16_fp32(N, X, Y);
}

void copy_fp32_u32(const unsigned int N, const float *X, uint32_t *Y) {
  __fallback_copy_fp32_u32(N, X, Y);
}

void copy_fp32_u16(const unsigned int N, const float *X, uint16_t *Y) {
  __fallback_copy_fp32_u16(N, X, Y);
}

void copy_fp32_u8(const unsigned int N, const float *X, uint8_t *Y) {
  __fallback_copy_fp32_u8(N, X, Y);
}

void copy_fp32_s16(const unsigned int N, const float *X, int16_t *Y) {
  __fallback_copy_fp32_s16(N, X, Y);
}

void copy_fp32_s8(const unsigned int N, const float *X, int8_t *Y) {
  __fallback_copy_fp32_s8(N, X, Y);
}

void scopy_int8_to_float32(const unsigned int N, const uint8_t *X,
                           const unsigned int incX, float *Y,
                           const unsigned int incY) {
  __fallback_scopy_uint8_to_float32(N, X, incX, Y, incY);
}

void scopy_int8_to_float32(const unsigned int N, const int8_t *X,
                           const unsigned int incX, float *Y,
                           const unsigned int incY) {
  __fallback_scopy_int8_to_float32(N, X, incX, Y, incY);
}

void sine(const unsigned int N, float *X, float *Y, float alpha) {
  __fallback_sine(N, X, Y, alpha);
}

void cosine(const unsigned int N, float *X, float *Y, float alpha) {
  __fallback_cosine(N, X, Y, alpha);
}

void inv_sqrt_inplace(const unsigned int N, float *X) {
  __fallback_inv_sqrt_inplace(N, X);
}

void ele_mul(const unsigned int N, const float *X, const float *Y, float *Z,
             float alpha, float beta, unsigned int i_stride,
             unsigned int o_stride) {
  nntrainer::avx2::ele_mul(N, X, Y, Z, alpha, beta, i_stride, o_stride);
}

void ele_add(const unsigned int N, const float *X, const float *Y, float *Z,
             float alpha, float beta, unsigned int i_stride,
             unsigned int o_stride) {
  nntrainer::avx2::ele_add(N, X, Y, Z, alpha, beta, i_stride, o_stride);
}

void ele_sub(const unsigned N, const float *X, const float *Y, float *Z,
             float alpha, float beta, unsigned int i_stride,
             unsigned int o_stride) {
  __fallback_ele_sub(N, X, Y, Z, alpha, beta, i_stride, o_stride);
}

void ele_div(const unsigned N, const float *X, const float *Y, float *Z,
             float alpha, float beta, unsigned int i_stride,
             unsigned int o_stride) {
  __fallback_ele_div(N, X, Y, Z, alpha, beta, i_stride, o_stride);
}

void saxpy(const unsigned int N, const float alpha, const float *X,
           const unsigned int incX, float *Y, const unsigned int incY) {
  __cblas_saxpy(N, alpha, X, incX, Y, incY);
}

void sgemv(const unsigned int TStorageOrder, bool TransA, const unsigned int M,
           const unsigned int N, const float alpha, const float *A,
           const unsigned int lda, const float *X, const unsigned int incX,
           const float beta, float *Y, const unsigned int incY) {
  __cblas_sgemv(TStorageOrder, TransA, M, N, alpha, A, lda, X, incX, beta, Y,
                incY);
}

float sdot(const unsigned int N, const float *X, const unsigned int incX,
           const float *Y, const unsigned int incY) {
  return __cblas_sdot(N, X, incX, Y, incY);
}

void scopy(const unsigned int N, const uint8_t *X, const unsigned int incX,
           uint8_t *Y, const unsigned int incY) {
  __fallback_scopy(N, X, incX, Y, incY);
}

void scopy(const unsigned int N, const int8_t *X, const unsigned int incX,
           int8_t *Y, const unsigned int incY) {
  __fallback_scopy(N, X, incX, Y, incY);
}

void scopy(const unsigned int N, const float *X, const unsigned int incX,
           float *Y, const unsigned int incY) {
  /// @note cblas_scopy is evoking SIGSEGV for some reason. Use custom
  /// implementation instead.
  // __cblas_scopy(N, X, incX, Y, incY);
  nntrainer::avx2::custom_scopy(N, X, incX, Y, incY);
}

void sscal(const unsigned int N, const float alpha, float *X,
           const unsigned int incX) {
  __cblas_sscal(N, alpha, X, incX);
}

float snrm2(const unsigned int N, const float *X, const unsigned int incX) {
  return __cblas_snrm2(N, X, incX);
}

void sgemm(const unsigned int TStorageOrder, bool TransA, bool TransB,
           const unsigned int M, const unsigned int N, const unsigned int K,
           const float alpha, const float *A, const unsigned int lda,
           const float *B, const unsigned int ldb, const float beta, float *C,
           const unsigned int ldc) {
  __cblas_sgemm(TStorageOrder, TransA, TransB, M, N, K, alpha, A, lda, B, ldb,
                beta, C, ldc);
}

unsigned int isamax(const unsigned int N, const float *X,
                    const unsigned int incX) {
  return __cblas_isamax(N, X, incX);
}

void transpose_matrix(const unsigned int M, const unsigned int N,
                      const float *src, unsigned int ld_src, float *dst,
                      unsigned int ld_dst) {
  nntrainer::avx2::transpose_matrix(M, N, src, ld_src, dst, ld_dst);
}

bool is_valid(const unsigned int N, const float *input) {
  return nntrainer::avx2::is_valid(N, input);
}

void calc_trigonometric_vals_dup(unsigned int N_half, float *angle, float *cos_,
                                 float *sin_, unsigned int alpha) {
  __fallback_calc_trigonometric_vals_dup(N_half, angle, cos_, sin_, alpha);
}

void swiglu(const unsigned int N, float *X, float *Y, float *Z) {
  nntrainer::avx2::swiglu(N, X, Y, Z);
}

float max_val(const unsigned int N, float *X) { return __fallback_max(N, X); }

void softmax(const unsigned int N, float *X, float *Y) {
  __fallback_softmax(N, X, Y);
}

void gemm_q4_0(const unsigned int M, const unsigned int N, const unsigned int K,
               const float *A, const unsigned int lda, const void *B,
               const unsigned int ldb, float *C, const unsigned int ldc) {
#ifdef ENABLE_GGML
  return __ggml_q4_0_8x8_q8_0_GEMM(M, N, K, A, lda, B, ldb, C, ldc);
#else
  return __fallback_gemm_q4_0(M, N, K, A, lda, B, ldb, C, ldc);
#endif
}

void gemm_q4_K(const unsigned int M, const unsigned int N, const unsigned int K,
               const float *A, const unsigned int lda, const void *B,
               const unsigned int ldb, float *C, const unsigned int ldc) {
#ifdef ENABLE_GGML
  return __ggml_q4_K_8x8_q8_K_GEMM(M, N, K, A, lda, B, ldb, C, ldc);
#else
  return __fallback_gemm_q4_K(M, N, K, A, lda, B, ldb, C, ldc);
#endif
}

float dot_q6_K_q8_K(const unsigned int K, const void *v_q6_K,
                    const void *v_q8_K) {
#ifdef ENABLE_GGML
  return __ggml_vec_dot_q6_K_q8_K(K, v_q6_K, v_q8_K);
#else
  return __fallback_dot_q6_K_q8_K(K, v_q6_K, v_q8_K);
#endif
}

float dot_q6_K_f32(const unsigned int K, const void *v_q6_K, const float *f) {
#ifdef ENABLE_GGML
  return __ggml_vec_dot_q6_K_f32(K, v_q6_K, f);
#else
  return __fallback_dot_q6_K_f32(K, v_q6_K, f);
#endif
}

void gemm_q6_K(const unsigned int M, const unsigned int N, const unsigned int K,
               const float *A, const unsigned int lda, const void *B,
               const unsigned int ldb, float *C, const unsigned int ldc) {
#ifdef ENABLE_GGML
  return __ggml_gemm_q6_K(M, N, K, A, lda, B, ldb, C, ldc);
#else
  return __fallback_gemm_q6_K(M, N, K, A, lda, B, ldb, C, ldc);
#endif
}

size_t quantize_q4_0(const float *src, void *dst, int64_t nrow,
                     int64_t n_per_row, const float *quant_weights) {
#ifdef ENABLE_GGML
  return __ggml_quantize_q4_0(src, dst, nrow, n_per_row, quant_weights);
#else
  return __fallback_quantize_q4_0(src, dst, nrow, n_per_row, quant_weights);
#endif
}

size_t quantize_q4_K(const float *src, void *dst, int64_t nrow,
                     int64_t n_per_row, const float *quant_weights) {
#ifdef ENABLE_GGML
  return __ggml_quantize_q4_K(src, dst, nrow, n_per_row, quant_weights);
#else
  return __fallback_quantize_q4_K(src, dst, nrow, n_per_row, quant_weights);
#endif
}

size_t quantize_q6_K(const float *src, void *dst, int64_t nrow,
                     int64_t n_per_row, const float *quant_weights) {
#ifdef ENABLE_GGML
  return __ggml_quantize_q6_K(src, dst, nrow, n_per_row, quant_weights);
#endif
  return __fallback_quantize_q6_K(src, dst, nrow, n_per_row, quant_weights);
}

void quantize_row_q6_K(const float *src, void *dst, int64_t k) {
#ifdef ENABLE_GGML
  __ggml_quantize_row_q6_K(src, dst, k);
#else
  __fallback_quantize_row_q6_K(src, dst, k);
#endif
}

void quantize_row_q8_K(const float *src, void *dst, int64_t k) {
#ifdef ENABLE_GGML
  __ggml_quantize_row_q8_K(src, dst, k);
#else
  __fallback_quantize_row_q8_K(src, dst, k);
#endif
}

void dequantize_row_q4_K(const void *x_raw, float *y, int64_t k) {
#ifdef ENABLE_GGML
  __ggml_dequantize_row_q4_K(x_raw, y, k);
#else
  __fallback_dequantize_row_q4_K(x_raw, y, k);
#endif
}

void dequantize_row_q6_K(const void *x, float *y, int64_t k) {
#ifdef ENABLE_GGML
  __ggml_dequantize_row_q6_K(x, y, k);
#else
  __fallback_dequantize_row_q6_K(x, y, k);
#endif
}

void dequantize_row_q8_K(const void *x, float *y, int64_t k) {
#ifdef ENABLE_GGML
  __ggml_dequantize_row_q8_K(x, y, k);
#else
  __fallback_dequantize_row_q8_K(x, y, k);
#endif
}

void repack_q4_0_to_q4_0_8(void *W, void *repacked_W, size_t data_size,
                           const unsigned int M, const unsigned int N) {
#ifdef ENABLE_GGML
  __ggml_repack_q4_0_to_q4_0_8(W, repacked_W, data_size, M, N);
#else
  __fallback_repack_q4_0_to_q4_0_8(W, repacked_W, data_size, M, N);
#endif
}

void repack_q4_K_to_q4_K_8(void *W, void *repacked_W, size_t data_size,
                           const unsigned int M, const unsigned int N) {
#ifdef ENABLE_GGML
  __ggml_repack_q4_K_to_q4_K_8(W, repacked_W, data_size, M, N);
#else
  __fallback_repack_q4_K_to_q4_K_8(W, repacked_W, data_size, M, N);
#endif
}

} /* namespace nntrainer */
