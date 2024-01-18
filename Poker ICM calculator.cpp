#include <iostream>
#include <string>
#include <cmath>
//#include <random>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;

//random_device rd;
//default_random_engine engine(rd());
//uniform_real_distribution<double> distribution(0.0, 1.0);

//double rand01()
//{
  //  return (double)distribution(engine);
//}

//Given chip count of each player, and ranking (e.g. {2,3,1} means player 0,1,2 rank 2,3,1)
//Output the probability
double probability_given_ranking(vector<double> chipcount, vector<int> ranking)
{
	if (chipcount.size() != ranking.size())
	{
		throw("Inputs must have same length");
	}
	double totalchip = 0;
	for (int i = 0; i < chipcount.size(); i++)
	{
		totalchip += chipcount[i];
	}//total chip

	double probability = 1.0;

	for (int i = 1; i <= ranking.size(); i++)
	{
		auto it = find(ranking.begin(), ranking.end(), i);
		int j = distance(ranking.begin(), it);//player j has rank i
		probability *= chipcount[j] / totalchip;//chance that player j is 1st place of remaining
		totalchip -= chipcount[j];
	}

	return probability;
}

vector<double> ICMcalculator(vector<double> chipcount, vector<double> prize)//chip count of each player //prize pool
{
	if (chipcount.size() != prize.size())
	{
		throw("Inputs must have same length");
	}

	sort(prize.begin(), prize.end(), [](double a, double b) {
		return a > b;
		});//Sort prize from large to small

	// Initialize a ranking vector {1,2,3,...,num of players}
	vector<int> ranking(chipcount.size());
	iota(ranking.begin(), ranking.end(), 1);

	// Initialize ICM value as 0 vector of num of players
	vector<double> ICMvalue(chipcount.size(), 0.0);

	do
	{
		double probability_of_this_ranking = probability_given_ranking(chipcount, ranking);
		for (int i = 0; i < prize.size(); i++)
		{
			ICMvalue[i] += prize[ranking[i] - 1] * probability_of_this_ranking;
		}
	} while (next_permutation(ranking.begin(), ranking.end()));//loop over all ranking permutation
	return ICMvalue;
}

int main()
{
	vector<double> sample_chip_count = { 125.0,5000.0,2000.0,800.0,1550.0,200.0,300.0,400.0,3000.0,50.0 };
	vector<double> sample_prize = { 10000.0,5000.0,3000.0,2000.0,1000.0,0.0,0.0,0.0,0.0,0.0 };
	vector<double> ICMvalue = ICMcalculator(sample_chip_count, sample_prize);
	for (int i = 0; i < sample_chip_count.size(); i++)
	{
		cout << "Player " << to_string(i + 1) << ": " << to_string(ICMvalue[i]) << "\n";
	}
}
