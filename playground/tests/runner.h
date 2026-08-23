/* 케이스 파일 러너 공통부.
 *
 * 각 test_0N.c 는 아래 셋을 정의한 뒤 이 헤더를 include 합니다.
 *   static const char *g_title;   단계 이름
 *   static const char *g_dir;     cases/<디렉터리>
 *   static int handle(t_case *c); 한 줄 처리. 아는 op 면 1, 모르면 0
 *
 * "그 단계에 주어진 케이스 파일이 전부 통과하면 그 단계 완료"가
 * 이 하네스의 합격 기준입니다.
 */

#ifndef RUNNER_H
# define RUNNER_H

# include <stdio.h>
# include <string.h>
# include "t.h"
# include "case.h"

/* 기대값 토큰들: "4.0" / "MISS" / "HIT" / "1,2,3" / "0xFF00FF" / "~0.01" */

/* 각 test_0N.c 가 아래에서 정의합니다 (tentative definition) */
static const char	*g_title;
static const char	*g_dir;
static int	handle(t_case *c);

static double	g_tol = 1e-6;

/* => 뒤에 붙은 ~<허용오차> 를 읽어 g_tol 을 세팅. 없으면 기본값 */
__attribute__((unused)) static void	read_tol(t_case *c, int at)
{
	int	i;

	g_tol = 1e-6;
	i = at;
	while (i < c->ntok)
	{
		if (c->tok[i][0] == '~')
			g_tol = cs_num(c->tok[i] + 1);
		i++;
	}
}

/* 교차 결과(t) 검증. MISS = 음수여야 함, HIT = 양수면 됨 */
__attribute__((unused)) static void	check_t(double got, const char *want, const char *what)
{
	char	b[192];

	if (cs_is(want, "MISS"))
	{
		if (got < 0.0)
			T_OK(what);
		else
		{
			snprintf(b, sizeof b, "빗나가야 하는데 t=%.9f 로 맞았다", got);
			T_NG(what, b);
		}
		return ;
	}
	if (cs_is(want, "HIT"))
	{
		if (got > 0.0)
			T_OK(what);
		else
			T_NG(what, "맞아야 하는데 빗나갔다");
		return ;
	}
	if (got < 0.0)
	{
		snprintf(b, sizeof b, "빗나갔다. want t=%s", want);
		T_NG(what, b);
		return ;
	}
	if (fabs(got - cs_num(want)) <= g_tol)
		T_OK(what);
	else
	{
		snprintf(b, sizeof b, "got t=%.9f  want t=%s", got, want);
		T_NG(what, b);
	}
}

__attribute__((unused)) static void	check_num(double got, const char *want, const char *what)
{
	char	b[192];

	if (fabs(got - cs_num(want)) <= g_tol)
		T_OK(what);
	else
	{
		snprintf(b, sizeof b, "got=%.9f  want=%s", got, want);
		T_NG(what, b);
	}
}

__attribute__((unused)) static void	check_vec(double x, double y, double z, const char *want,
		const char *what)
{
	double	wx;
	double	wy;
	double	wz;
	char	b[192];

	if (!cs_vec(want, &wx, &wy, &wz))
	{
		T_NG(what, "기대값이 x,y,z 형식이 아님");
		return ;
	}
	if (fabs(x - wx) <= g_tol && fabs(y - wy) <= g_tol
		&& fabs(z - wz) <= g_tol)
		T_OK(what);
	else
	{
		snprintf(b, sizeof b, "got=(%.6f, %.6f, %.6f)  want=(%s)",
			x, y, z, want);
		T_NG(what, b);
	}
}

__attribute__((unused)) static void	check_hex(unsigned int got, const char *want, const char *what)
{
	char	b[192];

	if (got == cs_hex(want))
		T_OK(what);
	else
	{
		snprintf(b, sizeof b, "got=0x%08X  want=%s", got, want);
		T_NG(what, b);
	}
}

int	main(void)
{
	char	names[CS_MAXFILES][CS_MAXNAME];
	char	path[CS_MAXNAME * 2];
	t_case	c;
	int		n;
	int		i;

	T_BEGIN(g_title);
	n = cs_list(g_dir, names);
	if (n == 0)
	{
		printf("  케이스 파일이 없습니다: %s\n", g_dir);
		return (1);
	}
	i = 0;
	while (i < n)
	{
		printf("\n  \033[36m[%s]\033[0m\n", names[i]);
		snprintf(path, sizeof path, "%s/%s", g_dir, names[i]);
		if (!cs_open(path))
		{
			T_NG(names[i], "파일을 열 수 없음");
			i++;
			continue ;
		}
		while (cs_next(&c))
		{
			if (!handle(&c))
			{
				char	b[128];

				snprintf(b, sizeof b, "%d행: 모르는 명령 '%s'",
					c.lineno, c.tok[0]);
				T_NG(names[i], b);
			}
		}
		cs_close();
		i++;
	}
	printf("\n  케이스 파일 %d개\n", n);
	return (T_END());
}

#endif
