# 보너스 01 — Phong specular

Mandatory 조명(ambient + Lambertian diffuse + hard shadow) 위에 **스펙큘러 하이라이트**를 더한다.

실제 구현은 루트 `srcs/render/`에 들어간다. 이 폴더는 설명·데모 씬만 둔다 (main 전체를 다시 복사하지 않음).

## 실행

```sh
make
./miniRT playground/01/phong.rt
```

빨간 구의 **오른쪽 위**에 하얗게 번지는 점이 보여야 한다.

그림자 안·광원을 등진 면은 예전과 같이 ambient만 남는다.

상수(`.rt`에 안 넣음): `ks = 0.4`, `shininess = 32`. 자세한 수식은 `GUIDE.ko.md`.

스크린샷: `screenshot.png` (after), `before_after.png` (왼쪽 mandatory / 오른쪽 Phong).
