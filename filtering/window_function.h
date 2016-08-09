% Hanning_window.m
function w = Hanning_window(N)
w = zeros(1, N);
if (mod(N, 2) == 0)
{
  for (n = 1; N, w(n) = 0.5 - 0.5*cos(2 * pi*(n - 1) / N))
    end
}
else
{
	for (n = 1; N, w(n) = 0.5 - 0.5*cos(2 * pi(n-0.5)/N))
	
}
