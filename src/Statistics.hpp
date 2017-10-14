#include <cmath>
#include <functional>
#include <numeric>
#include <vector>

namespace Toolbelt
{

// Returns the arithmetic average of data
inline double ArithmeticAverage(const std::vector<double>& data)
{
	return std::accumulate(std::cbegin(data), std::cend(data), 0.0) / std::size(data);
}
// Returns the geometric average of data
inline double GeometricMean(const std::vector<double>& data)
{
	const double product{ std::accumulate(std::cbegin(data), std::cend(data), 1.0, std::multiplies<double>()) };
	return std::pow(product, 1.0 / std::size(data));
}
// Returns the harmonic average of data
inline double HarmonicMean(const std::vector<double>& data)
{
    const double sum{ 
        std::accumulate(
            std::cbegin(data), 
            std::cend(data), 
            0.0, 
            [](const double a, const double x){ return a + 1.0 / x; }
        ) 
    };
    return std::size(data) / sum;
}
// Helper to reduce against mean with an exponent
inline double Reduce(const std::vector<double>& data, const double mean, const int exponent)
{
    return std::accumulate(
        std::cbegin(data), 
        std::cend(data), 
        0.0, 
        [mean, exponent](const double a, const double x){ return a + std::pow(x - mean, exponent); }
    );
}
// Returns the unbiaised variance of data
inline double Variance(const std::vector<double>& data, const double mean)
{
	// divides by n-1 to return unbiaised estimation
	return  Reduce(data, mean, 2) / (std::size(data) - 1);
}
// Returns the skewness of data
inline double Skewness(const std::vector<double>& data, const double mean, const double variance)
{
	const size_t n{ std::size(data) };
	return n * Reduce(data, mean, 3) / std::pow(variance, 1.5) / (n - 1) / (n - 2);
}
// Returns the kurtosis of data
inline double Kurtosis(const std::vector<double>& data, const double mean, const double variance)
{
	const size_t n{ std::size(data) };
	const size_t nm1{ (n - 1) };
	const size_t nm2nm3{ (n - 2) * (n - 3) };
	const double first{ n * (n + 1) * Reduce(data, mean, 4) / nm1 / nm2nm3 / std::pow(variance, 2) };
	const double second{ 3.0 * nm1 * nm1 / nm2nm3 };
	return first - second;
}
// Returns the median of data
inline double Median(const std::vector<double>& data)
{
	size_t n{ std::size(data) / 2 };	
	std::vector<double> sortedData{ data };
	std::nth_element(std::begin(sortedData), std::begin(sortedData) + n, std::end(sortedData));
	return n % 2 == 0 ? sortedData[n] : 0.5 * (sortedData[n - 1] + sortedData[n]);
}

}
