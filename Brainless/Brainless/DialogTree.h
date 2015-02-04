#ifndef INCLUDED_DIALOG_TREE_H
#define INCLUDED_DIALOG_TREE_H

#include <string>
#include <memory>
#include <vector>

struct DialogBranch
{
	typedef std::pair<std::string, DialogBranch> ChildBranch;

	std::string description;
	
	std::vector<ChildBranch> branches;
};

class DialogTree
{
public:

	DialogTree();
	DialogTree(const DialogTree &other);

	DialogTree& operator = (const DialogTree& other);

	void changeBranch(const DialogBranch &branch);
	void resetDialog(); // Resets dialog to root branch

	// Gets the dialog of the current branch
	std::string getCurrentDialog() const;
	// Gets the sub branches for the current branch
	const std::vector<DialogBranch::ChildBranch>& getCurrentOptions();
	// Get the name of the portrait texture for the character of this dialog
	std::string getPortraitTextureName() const;

	void printBranch(DialogBranch &branch, std::size_t indent);

	// Load the dialog tree from a file
	void loadDialogFile(const std::string &fileName);

private:
	
	std::size_t recursiveLoadFile(DialogBranch &branch, std::size_t indent, std::ifstream &reader);

	DialogBranch m_rootBranch;
	std::string m_portraitTextureName; // Portrait of the person you're talking to, also loaded from file

	const DialogBranch *m_currentBranch;
};

#endif