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
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;


class Giver
{

	string m_name;
	set<string> m_excludeList;
	string m_recipient;

public:
	Giver(string name, set<string> excludeList) :
	m_name(name), m_excludeList(excludeList), m_recipient("")
	{}


	string AssignRecipient(set<string> unassignedRecipients)
	{
		vector<string> availableRecipients(AvailableRecipients(unassignedRecipients));
		int randomIndex = rand() % availableRecipients.size();
		m_recipient = availableRecipients[randomIndex];

		return m_recipient;
	}

	bool CanAssignRecipient(set<string> unassignedRecipients)
	{
		return! AvailableRecipients(unassignedRecipients).empty();
	}

	string Filename()
	{
		return m_name;
	}

	string Recipient()
	{
		return m_recipient;
	}



	private:
		vector<string> AvailableRecipients(set<string> unassignedRecipients)
		{
			vector<string> difference_set;
			std::vector<string>::iterator it = set_difference(unassignedRecipients.begin(), unassignedRecipients.end(),
							m_excludeList.begin(), 	m_excludeList.end(), difference_set.begin());
			difference_set.resize(it - difference_set.begin());
			return difference_set;
		}


};


int main() {
	// build list of givers with excludedRecipients
	Giver John("John", set<string>{"Rosemary"});
	Giver Rosemary("Rosemary", set<string>{"John"});

	/// and many more
	vector<Giver> givers {John, Rosemary};
	set<string> recipients {"John", "Rosemary"};

	bool allAssigned = false;
	while(!allAssigned)
	{
		std::srand(std::time(0));

		bool failedAssignment = false;
		// assign next
		set<string> remainingRecipients = recipients;
		//for_each()
		for(int index = 0; index < givers.size(); index++)
		{
			auto nextGiver = givers[index];
			if(!nextGiver.CanAssignRecipient(remainingRecipients))
			{
				failedAssignment = false;
				break;
			}
			auto assignedRecipient = nextGiver.AssignRecipient(remainingRecipients);
			remainingRecipients.erase(assignedRecipient);
		}
		allAssigned = !failedAssignment;
	}

	// now create a bunch of files, name is giver, contents is recipient
	for(int index = 0; index < givers.size(); index++)
	{
		auto nextGiver = givers[index];
		auto fileName = nextGiver.Filename();
		std::ofstream out(fileName);
    	out << nextGiver.Recipient();
    	out.close();
	}

	return 0;
}
