//============================================================================
// Name        : SecretGiver.cpp
// Author      : John Klippenstein
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <algorithm>
#include <iterator>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <fstream>
#include <iostream>
#include <random>
#include <chrono>

using namespace std;

int GetRandomNumber(const int max)
{
	static unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	static minstd_rand0 generator(seed);
	std::uniform_int_distribution<int> distribution(0, max);
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
	m_name(std::move(name)), m_excludeList(extendSet(excludeList, m_name)), m_recipient("")
	{
	}


	string AssignRecipient(const set<string>& unassignedRecipients)
	{
		const vector<string> availableRecipients(AvailableRecipients(unassignedRecipients));

		const int randomIndex = GetRandomNumber(availableRecipients.size()-1);
		m_recipient = availableRecipients[randomIndex];

		return m_recipient;
	}

	bool CanAssignRecipient(const set<string>& unassignedRecipients) const
	{
		return! AvailableRecipients(unassignedRecipients).empty();
	}

	string Filename() const
	{
		return m_name;
	}

	string Recipient() const
	{
		return m_recipient;
	}



	private:
		vector<string> AvailableRecipients(const set<string>& unassignedRecipients) const
		{
			vector<string> difference_set(unassignedRecipients.size());
			std::vector<string>::iterator it = set_difference(unassignedRecipients.begin(), unassignedRecipients.end(),
							m_excludeList.begin(), 	m_excludeList.end(), difference_set.begin());
			difference_set.resize(it - difference_set.begin());
			return difference_set;
		}


};


int main() {
	// build list of givers with excludedRecipients


	vector<Giver> givers = {
			Giver ("John", set<string>{"Rosemary"}),
			Giver ("Rosemary", set<string>{"John"}),
			Giver("Cuthbert",{}),
			Giver("Philip", {})
	};

	/// and many more
	set<string> recipients {"John", "Rosemary", "Cuthbert", "Philip"};

	bool allAssigned = false;
	while(!allAssigned)
	{
		bool failedAssignment = false;
		// assign next
		set<string> remainingRecipients = recipients;
		for(auto& nextGiver : givers)
		{
			if(!nextGiver.CanAssignRecipient(remainingRecipients))
			{
				failedAssignment = true;
				std::cout << "Failed to find recipient for" << nextGiver.Filename();
				break;
			}
			auto assignedRecipient = nextGiver.AssignRecipient(remainingRecipients);
			remainingRecipients.erase(assignedRecipient);
		}
		allAssigned = !failedAssignment;
	}

	// now create a bunch of files, name is giver, contents is recipient
	for(const auto & nextGiver : givers)
	{
		auto fileName = nextGiver.Filename();
		std::ofstream out(fileName);
    	out << nextGiver.Recipient();
    	out.close();
	}

	return 0;
}
