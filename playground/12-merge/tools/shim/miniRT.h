/* 12단계 테스트 전용 연결 헤더 - 제출 대상 아님
 *
 * playground/tests/test_0N.c 는 "miniRT.h" 하나로 t_ray, hit_* 까지 찾고,
 * 학습 단계 때의 함수 모양(인자 5~7개)으로 부른다
 * srcs 로 옮기면서 norm(인자 4개) 때문에 모양이 바뀌었으니,
 * 테스트 코드는 그대로 두고 이름만 어댑터로 돌린다 (tools/contract_adapter.c)
 *
 * -I tools/shim 을 -I ../../includes 보다 앞에 둔다
 * render.h 안의 #include "miniRT.h" 는 render.h 옆(includes/)을 먼저 찾으니 진짜 팀 헤더로 간다
 */
#ifndef SHIM_MINIRT_H
# define SHIM_MINIRT_H

# include "render.h"

typedef struct s_sphere_lite
{
	t_vec3	center;
	double	radius;
}	t_sphere_lite;

int	shadow_test(t_vec3 p, t_vec3 light_pos, t_sphere_lite *blockers, int n);

/* 학습 단계 계약 이름 → 어댑터 */
# define hit_sphere		contract_hit_sphere
# define hit_plane		contract_hit_plane
# define hit_cylinder	contract_hit_cylinder

#endif
