/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shading.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 19:34:10 by akkim             #+#    #+#             */
/*   Updated: 2026/09/17 19:34:10 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/*
구의 법선 (표면에 수직인 단위벡터)

구 위의 점 P는 중심 C에서 정확히 r만큼 떨어져 있다
  → P - C 는 '중심에서 표면으로 뻗는' 방향이고, 길이가 r
  → 구는 어느 점에서 봐도 대칭이라 이 방향이 곧 표면에 수직
  → r로 나누면 길이 1 (normalize(P - C)와 같은 결과, sqrt 한 번 아낌)

         n ↗
          ●  P
        ╱
      ●  C
*/
t_vec3	sphere_normal(t_vec3 center, double radius, t_vec3 p)
{
	return (vec3_scale(vec3_sub(p, center), 1.0 / radius));
}

/*
램버트 코사인 법칙: 밝기 ∝ cos θ

둘 다 단위벡터면 dot(n, l) = |n||l|cos θ = cos θ
  θ = 0°   (광원을 정면으로 봄)  → 1   가장 밝다
  θ = 45°                        → 0.707
  θ = 90°  (빛이 표면을 스침)    → 0
  θ > 90°  (광원을 등짐)         → 음수 → 빛이 안 닿으니 0으로 자른다

같은 양의 빛이 비스듬히 들어오면 더 넓은 면적에 퍼져서 한 점당 밝기가 줄어든다
그 줄어드는 비율이 cos θ

light_dir은 '표면점 → 광원' 방향이어야 한다 (반대로 잡으면 밝고 어두운 곳이 뒤집힘)
정규화는 부르는 쪽에서 한다
*/
double	diffuse_factor(t_vec3 normal, t_vec3 light_dir)
{
	double	k;

	k = vec3_dot(normal, light_dir);
	// 등진 면: 클램프 안 하면 '음의 빛'이 ambient를 깎아먹는다
	if (k < 0.0)
		return (0.0);
	return (k);
}

/*
환경광 (ambient)

현실에선 빛이 벽/바닥에 튕겨서 광원을 등진 면에도 조금 닿는다
그걸 전부 계산하는 대신 '모든 면에 똑같이 조금' 이라는 상수로 근사한다
.rt의 `A 0.2 255,255,255` 가 ratio = 0.2, amb = (1,1,1)

물체색 x 빛색 은 '곱' (성분별)
  빛은 물체가 반사하는 성분만 통과시키는 필터라서
  빨강(1,0,0) x 파란빛(0,0,1) = 검정  ← 더하면 (1,0,1) 보라가 되어 틀린다
*/
t_color	apply_ambient(t_color obj, t_color amb, double ratio)
{
	return (vec3_scale(vec3_mul(obj, amb), ratio));
}

/*
표면점 P 하나의 최종 색 = ambient + diffuse

  ambient = 물체색 x 환경광색 x 비율
  diffuse = 물체색 x 광원색   x 밝기 x max(0, dot(n, l))

곱: 물체색과 빛색 (필터)
합: ambient와 diffuse (서로 다른 두 빛의 기여를 모은다)

더하면 1.0을 넘을 수 있으니 자르는 건 render_pixel 마지막에서 한다

※ subject 기준 mandatory에서는 광원 색(L의 R,G,B)을 안 쓴다고 되어 있다
  scenes/valid 는 전부 255,255,255라 결과는 같지만, 제출 전에 subject PDF로 확인할 것
*/
t_color	lighting(t_scene *scene, t_vec3 p, t_vec3 n, t_color obj_color)
{
	t_color	ambient;
	t_color	diffuse;
	t_vec3	light_dir;
	double	k;

	ambient = apply_ambient(obj_color, scene->ambient.color,
			scene->ambient.ratio);
	// '표면점 → 광원'. 광원 - P 순서
	light_dir = vec3_normalize(vec3_sub(scene->light.position, p));
	k = diffuse_factor(n, light_dir);
	diffuse = vec3_scale(vec3_mul(obj_color, scene->light.color),
			scene->light.brightness * k);
	return (vec3_add(ambient, diffuse));
}
