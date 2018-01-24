
#ifndef BUFFER_MGR_H
#define BUFFER_MGR_H

#include "MyDB_PageHandle.h"
#include "../../Catalog/headers/MyDB_Table.h"
#include "Node.h"


using namespace std;

class MyDB_BufferManager {

public:

	// THESE METHODS MUST APPEAR AND THE PROTOTYPES CANNOT CHANGE!

	// gets the i^th page in the table whichTable... note that if the page
	// is currently being used (that is, the page is current buffered) a handle 
	// to that already-buffered page should be returned
	MyDB_PageHandle getPage (MyDB_TablePtr whichTable, long i);

	// gets a temporary page that will no longer exist (1) after the buffer manager
	// has been destroyed, or (2) there are no more references to it anywhere in the
	// program.  Typically such a temporary page will be used as buffer memory.
	// since it is just a temp page, it is not associated with any particular 
	// table
	MyDB_PageHandle getPage ();

	// gets the i^th page in the table whichTable... the only difference 
	// between this method and getPage (whichTable, i) is that the page will be
	// pinned in RAM; it cannot be written out to the file
	MyDB_PageHandle getPinnedPage (MyDB_TablePtr whichTable, long i);

	// gets a temporary page, like getPage (), except that this one is pinned
	MyDB_PageHandle getPinnedPage ();

	// un-pins the specified page
	void unpin (MyDB_PageHandle unpinMe);

	// creates an LRU buffer manager... params are as follows:
	// 1) the size of each page is pageSize 
	// 2) the number of pages managed by the buffer manager is numPages;
	// 3) temporary pages are written to the file tempFile
	MyDB_BufferManager (size_t pageSize, size_t numPages, string tempFile);
	
	// when the buffer manager is destroyed, all of the dirty pages need to be
	// written back to disk, any necessary data needs to be written to the catalog,
	// and any temporary files need to be deleted
	~MyDB_BufferManager ();

	// FEEL FREE TO ADD ADDITIONAL PUBLIC METHODS

	void insertNode(Node * cur);
	Node * removeNode(Node * cur);
	Node * getVictim();
	bool evict();
	void reloadVictim(MyDB_Page *victim);
	void updateLRU(Node *cur);
	void readTable(string fileName, long offset, char* address);




private:
	char* buffer;

	Node *head;
	Node *tail;
	size_t pageSize;
	size_t numPages;
	string tempFile;
	int tempFileCurPosition;

	struct Hashing{
		size_t operator()(const pair<string,long> x)const
		{
			size_t code = 7;
			code = code * 31 + std::hash<string>()(x.first);
			code = code * 31 + std::hash<int>()(x.second);
			return code;
		}
	};

	unordered_map<pair<string, long>, Node*, Hashing> Map;
	queue<char*> unUsedPages;


};

#endif


