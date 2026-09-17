/* 케이스 파일 리더.
 *
 * 각 단계는 cases/<단계>/ 안의 .case 파일들로 평가합니다.
 * 파일 문법은 .rt 를 그대로 닮게 만들었습니다 - 식별자로 시작하는 줄,
 * 콤마로 묶은 벡터, '#' 주석. miniRT 파싱과 같은 감각으로 읽힙니다.
 *
 *   # 주석
 *   R 0,0,0 0,0,-1        <- 광선: 시작점 방향
 *   sp 0,0,-5 2 255,0,0   <- 도형: .rt 와 완전히 같은 문법 (지름!)
 *   => 4.0                <- 기대값
 *
 * 타입에 의존하지 않도록 벡터는 double 3개로 돌려줍니다.
 */

#ifndef CASE_H
# define CASE_H

# define CS_MAXTOK 16
# define CS_MAXLINE 512
# define CS_MAXFILES 64
# define CS_MAXNAME 256

typedef struct s_case
{
	char	raw[CS_MAXLINE];
	char	buf[CS_MAXLINE];
	char	*tok[CS_MAXTOK];
	int		ntok;
	int		lineno;
}	t_case;

/* cases/<dir> 안의 .case 파일 이름을 정렬해서 채운다. 개수 반환. */
int		cs_list(const char *dir, char names[CS_MAXFILES][CS_MAXNAME]);

int		cs_open(const char *path);
int		cs_next(t_case *c);          /* 유효한 줄이 있으면 1 */
void	cs_close(void);

/* "1,2,3" -> x,y,z. 실패하면 0 */
int		cs_vec(const char *s, double *x, double *y, double *z);
double	cs_num(const char *s);
unsigned int	cs_hex(const char *s);
int		cs_is(const char *tok, const char *s);

/* 기대값 토큰이 => 다음에 오는 위치. 없으면 -1 */
int		cs_arrow(t_case *c);

#endif
