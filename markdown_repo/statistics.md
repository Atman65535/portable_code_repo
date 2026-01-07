
---
- [x] Center Limit T
- [ ] Gaussian $\chi^2$, t, f distribution.

# Basic Calc

## Covariance
Linear and bi-linear property.

# Exponential Distribution
<center>No memory</center>
- $min{X_1, X_2, X_3}$ also follows exponential distribution
- $F(x) = 1 - e^{\lambda x}$ 

# CLT & LLN
## CLT
Independent identically distributed, limited variance and expectation.
Then we have
$$
\frac{\bar{X} - \mu}{\sigma/\sqrt n} \sim \mathcal N(0, 1)
$$
no matter the distribution is. 

## LLN
- Markov inequaion
$$
P(|X| \geqslant \epsilon) \leqslant \frac{E|X|^k}{\epsilon^k} 
$$

Then Chebbychev inequation

$$
P(|X - \miu| \geqslant \epsilon) \leqslant \frac{DX}{\epsilon^2}
$$

- Convergence in probability
$$
\lim_{n \to +\infty} \{|X_n - \theta | < \epsilon \} = 1 
$$
>[!warning] Not Expectation
# Normal Population
> [!danger] The $S^2$ and $\bar{X}$ 
> If given a normal distribution, they are independent, others are not.
> When it comes $\Sigma (X_i -\mu)^2$, it is **NOT INDEPENDENT** with $\bar{X}$


# Random Process Proof
- Strictly Stationary Process
For arbitary dimension distribution function:
$$
F(X_{1:n};t_{1:n}) = F(X_{1;n};t_{1:n}+\epsilon )
$$
However, this definition is bad for using

- Generalized Stationary Process
  1. The expectation exists
  2. The variance exists
  3. Self relative function only related to the time shift $\tau$

### About Proofs
- Gaussian Process
$$
\mathcal{N} : widesense \ stationary \  \Leftrightarrow strictly\ stationary
$$

>[!note] Gaussian
>So we always proof its generalized stationary property for proof strictly stationary
> And we should attention that this trait is only for Gaussian.

