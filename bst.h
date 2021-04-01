#include <iostream>
#include <functional>
using namespace std;

#define COUNT 10

void disp(auto first, auto last)
{
	while(first != last)
	{
		cout << *first << "\t";
		++first;
	}
	cout << "\n";
}


// Interface


template<typename T>
struct TreeNode
{
	T data;
	TreeNode *left;
	TreeNode *right;
	TreeNode *parent;
	TreeNode() : data(T()), left(nullptr), right(nullptr), parent(nullptr) { } //default constructor for the dummy node, stores the default value of the type T
	TreeNode(T data) : data(data), left(nullptr), right(nullptr), parent(nullptr) { }
};

template<typename T, typename Compare = less<T>>
class BST
{
	private:
	TreeNode<T> *root;
	Compare cmp; // will have operator() overloaded for comparision
	TreeNode<T> *dummy_end; //end iterator point to this that one location after the end of bst
	int cnt;
	void release(TreeNode<T> *root);
	void printUtil(TreeNode<T> *root, int space);
	static TreeNode<T> *min(TreeNode<T> *node);
	static TreeNode<T> *max(TreeNode<T> *node);
	static TreeNode<T> *inorder_successor(TreeNode<T> *node);
	static TreeNode<T> *inorder_predecessor(TreeNode<T> *node);
	public:
	BST() : root(nullptr), cmp(Compare()), cnt(0), dummy_end(new TreeNode<T>()){ } 
	~BST() { release(root); }
	class Iterator
	{
		private:
		TreeNode<T> *p_it_;
		public:
		Iterator(TreeNode<T> *p_it) : p_it_(p_it) { }
		Iterator& operator++() // pre
		{
			p_it_ = inorder_successor(p_it_);
			return *this;
		}
		Iterator operator++(int) // post
		{
			Iterator temp(*this);
			++*this;
			return temp;
		}
		Iterator& operator--() // pre
		{
			p_it_ = inorder_predecessor(p_it_);
			return *this;
		}
		Iterator operator--(int) // post
		{
			Iterator temp(*this);
			--*this;
			return temp;
		}
		T& operator*()
		{
			return p_it_->data;
		}
		bool operator==(const Iterator& rhs) const
		{
			if(p_it_ == nullptr && rhs.p_it_ == nullptr)
				return true;
			return p_it_ == rhs.p_it_;
		}
		bool operator!=(const Iterator& rhs) const
		{
			return !(*this == rhs);
		}
	};
	void insert(T x);
	Iterator search(T x);
	void remove(T x);
	void preorder();
	void inorder();
	void postorder();
	int totalcount() { return cnt; }
	int leafcount();
	int height();
	void print();
	Iterator min_element() { return Iterator(min(root)); }
	Iterator max_element() { return Iterator(max(root)); }
	Iterator begin() { return Iterator(min(root)); }
	// Iterator end() { return Iterator(nullptr); }
	Iterator end() { TreeNode<T>* max_node = max(root);
					 // cout << "1\n";
					 if(max_node->right == NULL)
					 	max_node->right = dummy_end;
					 // cout << "2\n";
					 return Iterator(dummy_end);
					}
};


// Implementation


template<typename T, typename Compare>
TreeNode<T> *BST<T, Compare>::min(TreeNode<T> *node)
{
	if(node == nullptr)
		return node;
	while(node->left)
		node = node->left;
	return node;
}

template<typename T, typename Compare>
TreeNode<T> *BST<T, Compare>::max(TreeNode<T> *node)
{
	if(node == nullptr)
		return node;
	while(node->right)
		node = node->right;
	return node;
}

template<typename T, typename Compare>
TreeNode<T> *BST<T, Compare>::inorder_successor(TreeNode<T> *node)
{
	if(node->right)
		return min(node->right);
	TreeNode<T> *p = node->parent;
	while(p && node == p->right)
	{
		node = p;
		p = p->parent;
	}
	return p;
}

template<typename T, typename Compare>
TreeNode<T> *BST<T, Compare>::inorder_predecessor(TreeNode<T> *node)
{
	if(node->left)
		return max(node->left);
	TreeNode<T> *p = node->parent;
	while(p && node == p->left)
	{
		node = p;
		p = p->parent;
	}
	return p;
}

template<typename T, typename Compare>
void BST<T, Compare>::insert(T x)
{
	if(root == nullptr)
	{
		root = new TreeNode<T>(x);
		++cnt;
	}
	else
	{
		TreeNode<T> *prev = nullptr;
		TreeNode<T> *curr = root;
		while(curr)
		{
			prev = curr;
			bool lesser = cmp(x, curr->data);
			if(!lesser && !cmp(curr->data, x))
			{
				return;
			}
			if(lesser)
			{
				curr = curr->left;
			}
			else
			{
				curr = curr->right;
			}
		}
		++cnt;
		TreeNode<T> *temp = new TreeNode<T>(x);
		if(cmp(x, prev->data))
		{
			prev->left = temp;
		}
		else
		{
			prev->right = temp;
		}
		temp->parent = prev;
	}
}

template<typename T, typename Compare>
void BST<T, Compare>::release(TreeNode<T> *root)
{
	if(root)
	{
		release(root->left);
		release(root->right);
		delete root;
	}
	return;
}

template<typename T, typename Compare>
void BST<T, Compare>::printUtil(TreeNode<T> *root, int space) 
{ 
    // Base case 
    if (root == nullptr) 
        return; 
  
    // Increase distance between levels 
    space += COUNT; 
  
    // Process right child first 
    printUtil(root->right, space); 
  
    // Print current node after space 
    // count 
    printf("\n"); 
    for (int i = COUNT; i < space; i++) 
        cout<<" "; 
    cout<<root->data<<"\n"; 
  
    // Process left child 
    printUtil(root->left, space); 
}
 
template<typename T, typename Compare>
// Wrapper over printUtil()
void BST<T, Compare>::print() 
{ 
    // Pass initial space count as 0 
    printUtil(root, 0); 
    printf("\n");
}

// template<typename T, typename Compare>
// BST<T, Compare>::Iterator BST<T, Compare>::end(){
// 	// BST<T, Compare>::Iterator it_max = max_element(); //But iterators abstract away the container, hence cannot access the right pointer of the max node :(
// 	// it_max->
// 	TreeNode<T>* max_node = max(root);
// 	max_node->right = dummy_end;
// 	return Iterator(dummy_end);
// }