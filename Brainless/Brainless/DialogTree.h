#ifndef INCLUDED_DIALOG_TREE_H
#define INCLUDED_DIALOG_TREE_H

#include <SFML\Graphics.hpp>
#include <string>
#include <memory>
#include <vector>

struct DialogBranch
{
	typedef std::unique_ptr<DialogBranch> BranchPtr;
	typedef std::pair<std::string, BranchPtr> ChildBranch;

	std::string description;
	
	std::vector<ChildBranch> branches;
};

class DialogTree
{
public:

	explicit DialogTree();

	void update(const sf::RenderWindow &gameWindow);
	void draw();

	void printBranch(DialogBranch &branch, std::size_t indent);

	// Load the dialog tree from a file
	void loadDialogFile(const std::string &fileName);

private:
	
	std::string recursiveLoadFile(DialogBranch &branch, std::size_t indent, std::ifstream &reader);

	DialogBranch m_rootBranch;
};

#endif