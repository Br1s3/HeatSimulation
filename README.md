# Program that simulates the heat equation


## Heat equation

```math
\begin{aligned}
	T_{(x, y, t)} &= \sum_{\substack{n=1 \\ m=1}}^N \frac{4}{L_x L_y} \int\limits_0^{L_x} \int\limits_0^{L_y} f(\varphi, \psi) \sin\left(n \frac{\pi}{L_x} \varphi\right) \sin\left(m \frac{\pi}{L_y} \psi\right) \, \mathrm{d}\varphi \, \mathrm{d}\psi \sin\left(n \frac{\pi}{L_x} x\right) \sin\left(m \frac{\pi}{L_y} y\right) \mathrm{e}^{-\alpha t \pi^2 \left(\frac{n^2}{L_x^2} + \frac{m^2}{L_y^2}\right)}
\end{aligned}
```

## normalization formula

```math
\begin{aligned}
	X_{new} &= \frac{X - X_{min}}{X_{max} - X_{min}}
\end{aligned}
```


## References

- https://fr.wikipedia.org/wiki/%C3%89quation_de_la_chaleur
- http://www.lmm.jussieu.fr/~lagree/COURS/MECAVENIR/cours4_eqchal_loc.pdf

## TODO:
- Add the console view
- Add the mp4 view
- Try with ODESolver
- Make the difference between cold and hot spots smoother
