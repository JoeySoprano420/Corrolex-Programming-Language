pub fn compute_derivative<F>(f: F, x: f64, dx: f64) -> f64
where
    F: Fn(f64) -> f64,
{
    (f(x + dx) - f(x)) / dx
}
