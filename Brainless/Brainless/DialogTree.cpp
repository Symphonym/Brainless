#include "DialogTree.h"
#include <fstream>

DialogTree::DialogTree()
{
}

void DialogTree::update(const sf::RenderWindow &gameWindow)
{

}
void DialogTree::draw()
{

}

void DialogTree::loadDialogFile(const std::string &fileName)
{
	std::ifstream reader(fileName);

	if (reader.is_open())
	{
		recursiveLoadFile(m_rootBranch, 0, reader);
		reader.close();
	}

	printBranch(m_rootBranch, 0);
}

#include <iostream>
void DialogTree::printBranch(DialogBranch &branch, std::size_t indent)
{
	std::string indentStr;

	for (int i = 0; i < indent; i++)
		indentStr += '\t';

	std::cout << indentStr << "DESC: " << std::endl;

	for (std::size_t i = 0; i < branch.branches.size(); i++)
	{
		std::cout << indentStr << "CHILD " << i << ": " << branch.branches[i].first << std::endl;
		printBranch(*branch.branches[i].second.get(), ++indent);
	}
}


// Returns the name of the choice it encountered before exiting
std::string DialogTree::recursiveLoadFile(DialogBranch &branch, std::size_t indent, std::ifstream &reader)
{
	std::string line;
	std::string optionName;
	std::string description;

	while (std::getline(reader, line))
	{
		// Remove indents (used for readability)
		std::string::size_type curIndent = line.find_last_of('\t');
		//line.erase(std::remove(line.begin(), line.end(), '\t'));

		if (curIndent == std::string::npos)
			curIndent = 0;

		// Skip empty lines
		if (line.empty())
			continue;

		char firstChar = line[curIndent];

		if (firstChar == '#')
		{
			// A new choice was found, add all the description text that's been read to the current branch
			if (!description.empty())
				branch.description = description;

			// Get name of the new choice
			optionName = line.substr(1, line.size());

			// If the next line has lower indent, it is earlier in the hierarchy, so end recursion
			//if (curIndent < indent)
				//return optionName;

			branch.branches.push_back(std::make_pair(optionName, std::move(DialogBranch::BranchPtr(new DialogBranch()))));
			recursiveLoadFile(*branch.branches.back().second.get(), curIndent, reader);
		}
		else
		{
			description += line;

			std::cout << "PEEK: " << reader.peek() << std::endl;

			// Check if the next line is earlier in the hierarchy
			std::size_t nextIndent = 0;

			if (reader.peek() == '\t')
			{
				while (reader.get() != '\t')
					++nextIndent;
			}


			// Next line is higher up in the hierarchy, so terminate further reading
			if (nextIndent < curIndent)
			{
				branch.description = description;
				return "";
			}

			for (std::size_t i = 0; i < nextIndent; i++)
				reader.unget();

			std::cout << "PEEK: " << reader.peek() << std::endl;
		}

		
	}

	return "";
}

