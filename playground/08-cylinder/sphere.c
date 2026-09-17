/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <akkim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 19:34:10 by akkim             #+#    #+#             */
/*   Updated: 2026/09/17 19:34:10 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/*
구와 광선이 만나는 t 구하기

1) 구:   중심 C에서 거리가 r인 점 P의 모임
         |P - C| = r
         루트가 불편하니 제곱하고, 길이² = 자기 자신과의 내적
         (P - C)·(P - C) = r²

2) 광선: P(t) = A + t·d   (A = 출발점, d = 방향)
         P 자리에 대입하고, t와 상관없는 A - C를 oc로 묶는다
         (oc + t·d)·(oc + t·d) = r²

3) 전개: 내적도 분배법칙이 된다. t는 숫자라 내적 밖으로 꺼낼 수 있다
         oc·oc + 2t(d·oc) + t²(d·d) = r²

4) 정리: t에 대한 2차방정식 at² + bt + c = 0
         a = d·d
         b = 2(d·oc)
         c = oc·oc - r²

5) 근:   t = (-b ± √D) / 2a,   D = b² - 4ac

반지름을 받는다. 팀 구조체 t_sphere는 diameter(지름)를 들고 있으니
부르는 쪽에서 반드시 sp.diameter / 2.0 으로 넘길 것

맞으면 t (양수), 안 맞으면 -1.0
*/
double	hit_sphere(t_ray r, t_vec3 center, double radius,
			double t_min, double t_max)
{
	t_vec3	oc;
	double	a;
	double	b;
	double	c;
	double	discriminant;
	double	sqrt_d;
	double	t;

	// 순서 주의: 출발점 - 중심. 뒤집으면(C - A) b의 부호가 뒤집혀서 t가 틀린다
	oc = vec3_sub(r.origin, center);

	// 방향이 단위벡터면 a = 1이지만, 정규화 안 된 광선이 들어와도 맞도록 생략하지 않는다
	a = vec3_dot(r.direction, r.direction);
	b = 2.0 * vec3_dot(r.direction, oc);
	// c < 0 이면 출발점이 구 안에 있다는 뜻 (중심까지 거리² < r²)
	c = vec3_dot(oc, oc) - radius * radius;

	// 판별식
	//   D < 0 : 광선이 구를 비껴간다 → 근이 없다
	//   D = 0 : 한 점에서 스친다 → 근 1개 (t1 = t2)
	//   D > 0 : 뚫고 지나간다 → 들어가는 점, 나오는 점
	discriminant = b * b - 4.0 * a * c;
	if (discriminant < 0.0)
		return (-1.0);
	sqrt_d = sqrt(discriminant);

	// a > 0, √D >= 0 이라서 항상 t1 <= t2
	// 그러니 작은 근(들어가는 점)부터 보면 '가장 가까운 교차점'이 먼저 잡힌다

	// t1: 들어가는 점
	// t_min보다 작으면 카메라 뒤(음수)거나, 방금 출발한 표면 자신(6단계 그림자 여드름)
	// t_max보다 크면 이미 더 가까운 물체가 있다(4단계 여러 구) / 광원보다 멀다(6단계 그림자)
	t = (-b - sqrt_d) / (2.0 * a);
	if (t > t_min && t < t_max)
		return (t);

	// t2: 나오는 점
	// t1이 범위 밖이어도 t2는 범위 안일 수 있다
	//   예) 카메라가 구 안에 있으면 t1 < 0 < t2 → 앞쪽 벽(t2)이 보여야 한다
	t = (-b + sqrt_d) / (2.0 * a);
	if (t > t_min && t < t_max)
		return (t);

	// 둘 다 범위 밖
	//   예) 구가 카메라 뒤에 있으면 t1 < t2 < 0 → D > 0 인데도 안 맞은 것
	return (-1.0);
}
