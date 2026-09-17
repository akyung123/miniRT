/* 학습용 초경량 테스트 하네스. 제출 대상 아님.
 * 프레임워크 없이 매크로 몇 개로 끝냅니다 - 42 과제에 외부 라이브러리를
 * 못 쓰기도 하고, 테스트 도구 배우는 게 목적이 아니라서요.
 *
 * 쓰는 법:
 *   #include "t.h"
 *   int main(void) { T_BEGIN("이름"); EXPECT_...; return (T_END()); }
 *
 * 종료 코드: 실패 0건이면 0, 아니면 1. run.sh가 이걸 봅니다.
 */

#ifndef T_H
# define T_H

# include <stdio.h>
# include <math.h>

static int	g_pass;
static int	g_fail;

# define T_EPS 1e-6

# define T_BEGIN(name) \
	do { g_pass = 0; g_fail = 0; \
		printf("\n\033[1m=== %s ===\033[0m\n", name); } while (0)

# define T_END() \
	(printf("---\n%s pass=%d fail=%d\033[0m\n", \
		g_fail ? "\033[31mFAIL\033[0m" : "\033[32mOK\033[0m", \
		g_pass, g_fail), g_fail != 0)

# define T_OK(what) \
	do { g_pass++; printf("  \033[32mok\033[0m   %s\n", what); } while (0)

# define T_NG(what, detail) \
	do { g_fail++; printf("  \033[31mFAIL\033[0m %s\n       %s\n", \
		what, detail); } while (0)

/* 참인가 */
# define EXPECT_TRUE(cond, what) \
	do { if (cond) T_OK(what); else T_NG(what, "조건이 거짓"); } while (0)

/* 정수 일치 */
# define EXPECT_INT(got, want, what) \
	do { long long g_ = (long long)(got), w_ = (long long)(want); \
		if (g_ == w_) T_OK(what); \
		else { char b_[128]; snprintf(b_, sizeof b_, \
			"got=%lld want=%lld", g_, w_); T_NG(what, b_); } } while (0)

/* 16진 일치 (색상용) */
# define EXPECT_HEX(got, want, what) \
	do { unsigned g_ = (unsigned)(got), w_ = (unsigned)(want); \
		if (g_ == w_) T_OK(what); \
		else { char b_[128]; snprintf(b_, sizeof b_, \
			"got=0x%08X want=0x%08X", g_, w_); T_NG(what, b_); } } while (0)

/* 실수 근사 일치 */
# define EXPECT_NEAR(got, want, tol, what) \
	do { double g_ = (double)(got), w_ = (double)(want); \
		if (fabs(g_ - w_) <= (tol)) T_OK(what); \
		else { char b_[128]; snprintf(b_, sizeof b_, \
			"got=%.9f want=%.9f (오차 %.2e 허용)", g_, w_, (double)(tol)); \
			T_NG(what, b_); } } while (0)

/* x/y/z 필드를 가진 아무 구조체나 (t_vec3 / t_color) */
# define EXPECT_VEC3(v, X, Y, Z, tol, what) \
	do { double dx_ = (v).x - (X), dy_ = (v).y - (Y), dz_ = (v).z - (Z); \
		if (fabs(dx_) <= (tol) && fabs(dy_) <= (tol) && fabs(dz_) <= (tol)) \
			T_OK(what); \
		else { char b_[192]; snprintf(b_, sizeof b_, \
			"got=(%.6f, %.6f, %.6f) want=(%.6f, %.6f, %.6f)", \
			(double)(v).x, (double)(v).y, (double)(v).z, \
			(double)(X), (double)(Y), (double)(Z)); T_NG(what, b_); } } while (0)

/* 범위 안인가 (0~1 색상 검증 등) */
# define EXPECT_RANGE(got, lo, hi, what) \
	do { double g_ = (double)(got); \
		if (g_ >= (lo) && g_ <= (hi)) T_OK(what); \
		else { char b_[128]; snprintf(b_, sizeof b_, \
			"got=%.9f, 허용 범위 [%.3f, %.3f]", g_, (double)(lo), \
			(double)(hi)); T_NG(what, b_); } } while (0)

#endif
