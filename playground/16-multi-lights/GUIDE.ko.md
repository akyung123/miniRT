# 16단계 — 색이 있는 여러 점광원

**브랜치**: `learn/16-multi-lights` (`learn/12-merge`에서 분기)

mandatory는 `L`이 정확히 하나였습니다. 여기서는 `L`을 여러 줄 쓸 수 있고, 각 광원의 `R,G,B`가 diffuse에 곱해집니다.

```
L <x,y,z> <brightness 0-1> <R,G,B>
```

`A`와 `C`는 여전히 하나, `L`은 **하나 이상**입니다. 기존 흰 광원 한 개 씬은 그대로 됩니다.

```sh
cd playground/16-multi-lights
make
./miniRT scenes/multi_lights.rt
```

데모는 흰 구를 빨강·초록·파랑 광원이 옆에서 비춥니다. 구 표면에서 색이 섞여 보여야 합니다.

각 광원은 자기 그림자를 따로 검사합니다. 한 광원에 가려져도 다른 광원은 더해집니다.
