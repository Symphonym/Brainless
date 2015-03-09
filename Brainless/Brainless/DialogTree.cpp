#include "DialogTree.h"
#include <iostream>
#include <fstream>
#include "ResourceLoader.h"

DialogTree::DialogTree()
:
m_currentBranch(&m_rootBranch)
{

}
DialogTree::DialogTree(const DialogTree &other)
:
m_currentBranch(nullptr)
{
	operator=(other);

}

DialogTree& DialogTree::operator= (const DialogTree& other)
{
	m_rootBranch = other.m_rootBranch;
	m_currentBranch = &m_rootBranch;
	m_portraitTextureName = other.m_portraitTextureName;
	return *this;
}

void DialogTree::changeBranch(const DialogBranch &branch)
{
	m_currentBranch = &branch;
}
void DialogTree::resetDialog()
{
	m_currentBranch = &m_rootBranch;
}
std::string DialogTree::getCurrentDialog() const
{
	return m_currentBranch->description;
}
const std::vector<DialogBranch::ChildBranch>& DialogTree::getCurrentOptions()
{
	return m_currentBranch->branches;
}
std::string DialogTree::getPortraitTextureName() const
{
	return m_portraitTextureName;
}

void DialogTree::loadDialogFile(const std::string &fileName)
{
	std::ifstream reader(fileName);
	m_rootBranch.branches.clear();
	m_rootBranch.description = "";
	if (reader.is_open())
	{
		std::getline(reader, m_portraitTextureName);

		recursiveLoadFile(m_rootBranch, 0, reader);
		reader.close();
	}
}

void DialogTree::printBranch(DialogBranch &branch, std::size_t indent)
{
	std::string indentStr;

	for (int i = 0; i < indent; i++)
		indentStr += '\t';

	if (!branch.description.empty())
		std::cout << indentStr << "Description: " << branch.description <<  std::endl;

	++indent;
	for (std::size_t i = 0; i < branch.branches.size(); i++)
	{
		std::cout << indentStr << "Answer " << i << ": " << branch.branches[i].first << std::endl;
		printBranch(branch.branches[i].second, indent);
	}
}

// Returns the name of the choice it encountered before exiting
std::size_t DialogTree::recursiveLoadFile(DialogBranch &branch, std::size_t indent, std::ifstream &reader)
{
	std::string line;
	std::string optionName;
	std::string description;

	while (std::getline(reader, line))
	{
		// Skip empty lines
		if (line.empty())
			continue;

		// Check if the next line is earlier in the hierarchy
		std::size_t nextIndent = 0;

		// Peek first because we don't want to unget incorrectly
		if (reader.peek() == '\t')
		{
			while (reader.get() == '\t')
				++nextIndent;
			reader.unget(); // Unget the fail condition on the loop
		}

		char firstChar = line[0];

		// Lines starting with '#' are choices
		if (firstChar == '#')
		{
			// Parse the text of the choice
			optionName = line.substr(1, line.size());
			branch.description = description;

			// Push this new choice as a branch to the parent branch
			branch.branches.push_back(std::make_pair(optionName, DialogBranch()));
			
			// If the next line has more indention, then it has nested information that we can recurse into
			// The indentation of the nextline is propagated upwards from the recursion
			if (nextIndent > indent)
				nextIndent = recursiveLoadFile(branch.branches.back().second, nextIndent, reader);
			
			// If the next line is higher up in the hierarchy, do not recurse further
			if (nextIndent < indent)
			{
				branch.description = description;
				return nextIndent;
			}
		}

		// Everything else is description text (not choices)
		else
		{
			description += line;

			// If the next line is higher up in the hierarchy, do not recurse further
			if (nextIndent < indent)
			{
				branch.description = description;
				return nextIndent;
			}
		}
	}

	return 0;
}

