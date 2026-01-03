# Complex Function
contains real part and imagine part.
- argz 
	-  $\arctan{z} + \pi$ or $-\pi$ , $2k \pi$ 
	- not continuious on negative real axis
- Cauchy-Riemann
	- $\frac{\partial u}{\partial x} =\frac{\partial v}{\partial y} \ \frac{\partial u}{\partial y} = - \frac{\partial v}{\partial x}$  
	- $f'(x) = \frac{\partial f}{\partial x}$ 


# Cauchy Integral and High order Deviate
- if analytic:
$$
\int_C f(z)dz = 0
$$
$$
\int_c \frac{f(\zeta)}{\zeta - z}d\zeta = 2\pi i f(z)
$$
- then cauchy high order deviate
$$
f^{(n)}(z) = \frac{n!}{2 \pi i} \int_C \frac{f(\zeta)}{(\zeta - z)^{n+1}}d\zeta
$$

# Laurent and Residual
- Laurent: series presentation of function in ring regions.
- Residuals $c_{-1}$
$$

Res[f(z), s_k] = \frac{1}{(k-1)!} [\frac{d(z-z_0)^kf(z)}{dz} ]^{(k -1)}

$$
