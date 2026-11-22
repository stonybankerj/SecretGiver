//============================================================================
// Name        : SecretGiver.cpp
// Author      : John Klippenstein
// Version     :
// Copyright   : Your copyright notice
// Description : Uses schema2cpp to load givers and receivers from JSON
//============================================================================

#include <iostream>
#include <algorithm>
#include <iterator>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <fstream>
#include <random>
#include <chrono>

#include "InputConfig.hpp"

using namespace std;

size_t GetRandomNumber(const size_t max)
{
	static unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	static minstd_rand0 generator(seed);
	std::uniform_int_distribution<size_t> distribution(0, max);
	return distribution(generator);

}

set<string> extendSet(set<string> original, const string& extension)
{
	original.insert(extension);
	return original;
}

class Giver
{

	const string m_name;
	const set<string> m_excludeList;
	string m_recipient;

public:
	Giver(string  name, const set<string>& excludeList) :
	m_name(std::move(name)), m_excludeList(extendSet(excludeList, m_name))
	{
	}


	optional<string> AssignRecipient(const set<string>& unassignedRecipients)
	{
		const vector availableRecipients(AvailableRecipients(unassignedRecipients));
		if(availableRecipients.empty())
		{
			std::cout << "Failed to find recipient for" << m_name;
			return nullopt;
		}

		const auto randomIndex = GetRandomNumber(availableRecipients.size()-1);
		m_recipient = availableRecipients[randomIndex];

		return m_recipient;
	}

	void Write() const
	{
		ofstream output(m_name);
		output << m_recipient;
		output.close();

	}

	private:
		[[nodiscard]] vector<string> AvailableRecipients(const set<string>& unassignedRecipients) const
		{
			vector<string> difference_set(unassignedRecipients.size());
			const auto it = set_difference(unassignedRecipients.begin(), unassignedRecipients.end(),
							m_excludeList.begin(), 	m_excludeList.end(), difference_set.begin());
			difference_set.resize(it - difference_set.begin());
			return difference_set;
		}

};

bool AssignRecipients(vector<Giver>& givers, const set<string>& recipients)
{
	set<string> remainingRecipients = recipients;
	for(auto& nextGiver : givers)
	{
		if(auto assignedRecipient = nextGiver.AssignRecipient(remainingRecipients))
		{
			remainingRecipients.erase(*assignedRecipient);
		}
		else
		{
			return false;
		}
	}
	return true;
}

pair<vector<Giver>, const set<string>> parseConfig(const InputConfig& config)
{
	vector<Giver> givers;
	for (const auto& g : config.givers)
	{
		set excludes(g.excludes.begin(), g.excludes.end());
		givers.emplace_back(g.name, excludes);
	}

	const set<string> recipients {config.recipients.begin(), config.recipients.end()};
	return {givers, recipients};
}

void assignAllRecipients(vector<Giver>& givers, const set<string>& recipients) {
	bool allAssigned = false;
	while(!allAssigned)
	{
		allAssigned = AssignRecipients(givers, recipients);
	}
}

int main() {
	// Load givers and receivers from the embedded schema2cpp-generated data
	const InputConfig& config = defaultInputConfig();
	auto [givers, recipients] = parseConfig(config);

	assignAllRecipients(givers, recipients);

	// now create a bunch of files, name is giver, contents is recipient
	for(const auto & nextGiver : givers)
	{
		nextGiver.Write();
	}

	return 0;
}
