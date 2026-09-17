# 보너스 02 — Checkerboard (color disruption)

교차점의 알베도(물체색)를 조명 전에 두 색이 번갈아 나오는 **체커보드**로 바꾼다.

Mandatory 조명(ambient + Lambertian diffuse + hard shadow)은 그대로다. Phong(#8)과 독립.

## 수식

평면(바닥): 접선 기저로 `(u, v)` 를 잡고

```
odd = (floor(u / scale) + floor(v / scale)) & 1
albedo = odd ? color2 : color1
```

수평 바닥(`normal ≈ (0,1,0)`)에서는 `u,v` 가 곧 `z,x` 이라 전형적인 `floor(x/s)+floor(z/s)` 와 같다.

## 켜는 방법

물체 줄 **맨 뒤**에 선택 키워드 `ck` (mandatory 필드 개수는 그대로):

```
pl <point> <normal> <R,G,B> ck <scale> <R,G,B>
sp <center> <diameter> <R,G,B> ck <scale> <R,G,B>
cy <center> <axis> <diameter> <height> <R,G,B> ck <scale> <R,G,B>
```

`ck` 가 없으면 예전과 같다. 모르는 identifier 나 `ck` 가 아닌 여분 토큰은 여전히 파싱 실패.

데모에서 고른 값: 바닥 `scale=1.5`, 크림 `232,220,198` / 차콜 `28,26,32`. 빨간 구는 구면 UV, `scale=0.55`.

## 실행

```sh
make
./miniRT playground/02/checkerboard.rt
```

체커 바닥 위에 빨간 체크 구 + 파란 단색 구가 보여야 한다.

자세한 수식·함정: `GUIDE.ko.md`. 스크린샷: `screenshot.png`.
