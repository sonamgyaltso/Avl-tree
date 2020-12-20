#pragma once

#include <iostream>
#include <algorithm>  // std::max
#include <cstdlib>    // std::abs
#include <stack>
#include <vector>
#include <cassert>

using namespace std;

template<typename TKey, typename TValue>
class avltree
{
private:
  struct NODE
  {
    TKey   Key;
    TValue Value;
    int    Height;
    NODE*  Left;
    NODE*  Right;
  };

  NODE* Root;  // pointer to root node of tree (nullptr if empty)
  int   Size;  // # of nodes in the tree (0 if empty)

  // _inorder does the actual inorder traversal and output 
  // to console.  Each key is output to the console followed
  // by " ", including the last key.
//**********************************************************************************
void _inorder(NODE* cur)
  {
    if (cur == nullptr)
      return;
    else
    {
      _inorder(cur->Left);
      cout << "(" << cur->Key << "," << cur->Value << "," << cur->Height << ")" << " ";
      _inorder(cur->Right);
    }
  }
//**********************************************************************************
void _inorder_keys(NODE* cur, std::vector<TKey>& V)
  {
    if (cur == nullptr)
      return;
    else
    {
      _inorder_keys(cur->Left, V);
      V.push_back(cur->Key);
      _inorder_keys(cur->Right, V);
    }
  }
//**********************************************************************************
void _inorder_values(NODE* cur, std::vector<TValue>& V)
  {
    if (cur == nullptr)
      return;
    else
    {
      _inorder_values(cur->Left, V);
      V.push_back(cur->Value);
      _inorder_values(cur->Right, V);
    }
  }
//**********************************************************************************
void _inorder_heights(NODE* cur, std::vector<int>& V)
  {
    if (cur == nullptr)
      return;
    else
    {
      _inorder_heights(cur->Left, V);
      V.push_back(cur->Height);
      _inorder_heights(cur->Right, V);
    }
  }
   // _copytree:
  // Given a tree, inserts the (key, value) pairs into *this* tree.
//**********************************************************************************
void _copytree(NODE* cur)
  {
    if (cur == nullptr)
      return;
    else
    {// to copy and build an exact copy, we need to "visit" cur
      // first and insert that (key, value) pair first.  That way
      // all the nodes to the left will properly go to the left,
      // all the node to the right will properly go to the right.
      insert(cur->Key, cur->Value);
	  _copytree(cur->Left);
      _copytree(cur->Right);
    }
  }
//**********************************************************************************
 void avlsearchtree_(NODE* cur){
	  if(cur==NULL){
		  return;
	  }
	  else{
		 avlsearchtree_(cur->Left);
		  avlsearchtree_(cur->Right);
		  delete cur;
	  }
	  
  }
//**********************************************************************************
//update height after the rotation occurs
void updateHeight( NODE* N){
//if both null heigh is 0
 if(N->Left==NULL && N->Right==NULL){
	   N->Height=0;
   }
// if left is null then height is right height
   else if(N->Left==NULL){
	   N->Height=N->Right->Height+1;
	   }
//if right null then left is height
	else if(N->Right==NULL){
	   N->Height=N->Left->Height+1;
	}
//else then add both of em up for height 
	else{
		N->Height=max(N->Left->Height,N->Right->Height)+1;
	}
}
//**********************************************************************************
void _RightRotate(NODE* Parent, NODE* N)
  { 
    assert(N != nullptr);  // pre-conditions: N and L must exist
    assert(N->Left != nullptr);
	//l is left kid of N 
	//B is is the left right of n 
    NODE* L=N->Left;
    NODE* B=L->Right;
	//n left is now b 
	//l becomes parent of N
	N->Left=B;
	L->Right=N;
	//if parent null then root becomes l
	if(Parent==NULL){
	Root=L;
	}
	//if parrent key greater than l 
	//ll becomes left
   else if(L->Key < Parent->Key){
	   Parent->Left=L;
   }
   //else right
   else{
	   Parent->Right=L;
   }
   //heights updated
  updateHeight(N);
  updateHeight(L);
	}
//**********************************************************************************
void _LeftRotate(NODE* Parent, NODE* N)
{  assert(N != nullptr);  // pre-conditions: N and R must exist
   assert(N->Right != nullptr);
   //l is n right
   //b is n right left
   NODE* L=N->Right;
   NODE* B=L->Left;
   //n right is now b 
   //l left is now N
   N->Right=B;
   L->Left=N;
   //l is root if parent null
   	if(Parent==NULL){
	Root=L;
	}
	//if parrent key greater than l 
	//ll becomes left
	 else if(L->Key < Parent->Key){
	   Parent->Left=L;
   }
   //else becomes right
   else{
	   Parent->Right=L;
   }
   //height update
  updateHeight(N);
  updateHeight(L);
 }
//**********************************************************************************
 void _rotatetofix(NODE* parent,NODE* node)
 {   int hL = (node->Left == nullptr) ? -1 : node->Left->Height;
     int hR = (node->Right == nullptr) ? -1 : node->Right->Height;
	//if left height greater than right
	if(hL>hR){
	//heights for left kids
		int hLl = (node->Left->Left == nullptr) ? -1 : node->Left->Left->Height;
		int hlR = (node->Left->Right == nullptr) ? -1 : node->Left->Right->Height;
	//if hllr is greater than hlr than we know we 
	//only need a right rotate
		if(hLl > hlR){
			_RightRotate(parent,node);
		}
	//else than a left right 
		else{
			_LeftRotate(node,node->Left);
			_RightRotate(parent,node);
		}
	}
	//if hr is greater than hl
	else {
	//kids for hr
		int hRl = (node->Right->Left == nullptr) ? -1 : node->Right->Left->Height;
		int hRR = (node->Right->Right == nullptr) ? -1 : node->Right->Right->Height;
	//if hrr greater than hrl than 
	//itll just be a left 	
		if(hRR > hRl){
			_LeftRotate(parent,node);
			}
	//else right left
		else{
			_RightRotate(node,node->Right);
			_LeftRotate(parent,node);
		}
	}
}
//**********************************************************************************
struct NODE *lca(NODE* cur, TKey key, TKey key2) 
{ 
	//if root null then null
    if (cur == NULL) {
	return NULL; 
  }
    //if cur key greater than both key 
    //than go left
    if (cur->Key> key && cur->Key > key2) {
        return lca(cur->Left, key, key2); 
	}
	//else go right 
    if (cur->Key < key && cur->Key < key2) {
        return lca(cur->Right, key, key2); 
  }
    return cur; 
} 
//**********************************************************************************
int level(NODE* cur,TKey key,int levels){
	//root null than -1
	if(cur==NULL){
		return -1;
	}
	//else if cur is key than level 0
	if(cur->Key==key){
		return levels;
	}
	//if not than go left
	int l=level(cur->Left,key,levels+1);
	//if it isnt -1 than return
	if(l!=-1){
		return l;
	}
	//go right same procedure
	int r=level(cur->Right,key,levels+1);
	return r;
}
//**********************************************************************************
public:
//**********************************************************************************
  // default constructor:
  // Creates an empty tree.
avltree()
  {
    Root = nullptr;
    Size = 0;
  }
//**********************************************************************************  
  // copy constructor:
avltree(avltree& other)
  {
    Root = nullptr;
    Size = 0;

    _copytree(other.Root);
  }
virtual ~avltree()
  {
    avlsearchtree_(Root);
    
  }
//**********************************************************************************
  // size:
  // Returns the # of nodes in the tree, 0 if empty.
int size()
  {
    return Size;
  }
//**********************************************************************************  
  int height()
  {
    if (Root == nullptr)
      return -1;
    else
      return Root->Height;
  }
//**********************************************************************************
 int distance(TKey key,TKey key2){
	//if key and key 2 same than check if it exist if
	//not return -1 else 0
 if(key==key2){
	if(search(key)==NULL || search(key2) ==NULL){
		return -1;
	}
	else{
		return 0;
	}
 }
	//k becomes the lowest common ancestor of key and key2
  NODE* k=lca(Root,key,key2);
	//add them up for distance  
  int d1=level(k,key,0);
  int d2=level(k,key2,0);
  int tot=d1+d2;
  if (tot==-1){
	  tot=1;
  }
  return tot;
  }
//**********************************************************************************
  // search:
  // Searches the tree for the given key, returning a pointer to the 
  // value if found and nullptr if not.  We return a pointer so the caller
  // may update the underlying value if 
  TValue* search(TKey key)
  {
    NODE* cur = Root;
    while (cur != nullptr)
    {
      if (key == cur->Key)  // already in tree
      {
        return &cur->Value;
      }

      if (key < cur->Key)  // search left:
      {
        cur = cur->Left;
      }
      else
      {
        cur = cur->Right;
      }
    }//while  
	// if get here, not found
    return nullptr;
  }
//********************************************************************************** 
  // insert
  // Inserts the given (key, value) pair into the tree; if the key has 
  // already been inserted then the function returns without changing 
  // the tree.
  // // height
  // Returns the height of tree; the height of an empty tree is
  // defined as -1.
  int _height(NODE *cur){
     if (cur==NULL){
        return -1;
        }
      else {
       int leftHeight = _height(cur->Left);
       int  rightHeight = _height(cur->Right);
         if(leftHeight>rightHeight){
            return 1+leftHeight;
            }
            else{
               return 1+rightHeight;
               }
         }
     }
//**********************************************************************************
  void insert(TKey key, TValue value)
  {
    NODE* prev = nullptr;
    NODE* cur = Root;
    // stack the nodes we visit so we can walk back up
    // the search path later, adjusting heights:
    stack<NODE*> nodes;
    // 1. Search to see if tree already contains key:
    while (cur != nullptr)
    {
      if (key == cur->Key)  // already in tree
        return;

      nodes.push(cur);  // stack so we can return later:

      if (key < cur->Key)  // search left:
      {
        prev = cur;
        cur = cur->Left;
      }
      else
      {
        prev = cur;
        cur = cur->Right;
      }
    }//while
    // 2. if we get here, key is not in tree, so allocate
    // a new node to insert:
    NODE* newNode;
    newNode = new NODE();
    newNode->Key = key;
    newNode->Value = value;
    newNode->Height = 0;  // leaf node -> sub-tree of height 0:
    newNode->Left = nullptr;
    newNode->Right = nullptr;
    // 3. link in the new node:
    // NOTE: cur is null, and prev denotes node where
    // we fell out of the tree.  if prev is null, then
    // the tree is empty and the Root pointer needs 
    // to be updated.
    if (prev == nullptr)
      Root = newNode;
    else if (key < prev->Key)
      prev->Left = newNode;
    else
      prev->Right = newNode;
    // 4. update size:
    Size++;
	
    // 5. walk back up tree using stack and update heights.
    while(!nodes.empty())
	{
         cur=nodes.top();
         nodes.pop();
		//heights of hl and hr 
		 int hl=_height(cur->Left);
         int hr=_height(cur->Right);
		 int newH=1+max(hl,hr);
		 if (cur->Height == newH) { // didn't change, so no need to go further:
			break;
		  }
		  //if balanced than return height
		  if(std::abs(hl-hr)<= 1){
		  cur->Height=newH;
		  }
		  //if its inbalanced then rotate to fix
		  //if node empty than parent is null
		  else if(hl>hr || hl<hr){
			  if(nodes.empty()){
				_rotatetofix(NULL,cur);
			  }
			  else{
				_rotatetofix(nodes.top(),cur);
				  }
			  }
		}//while
	return;
  }
//**********************************************************************************
  // inorder:
  // Performs an inorder traversal of the tree, outputting
  // the keys to the console.
  void inorder()
  {
    cout << "Inorder: ";

    _inorder(Root);

    cout << endl;
  }
//**********************************************************************************
  std::vector<TKey> inorder_keys()
  {
    std::vector<TKey>  V;

    _inorder_keys(Root, V);

    return V;
  }
//**********************************************************************************
  std::vector<TValue> inorder_values()
  {
    std::vector<TValue>  V;

    _inorder_values(Root, V);

    return V;
  }
//**********************************************************************************
  std::vector<int> inorder_heights()
  {
    std::vector<int>  V;

    _inorder_heights(Root, V);

    return V;
  }

//**********************************************************************************  //
  // rotate:
  //
  // The idea of this function is to rotate the tree anywhere you want,
  // for testing purposes.  The heights are not checked, and the AVL
  // condition is ignored.  If you call to rotate the tree around the
  // key 123, the tree is rotated.
  //
  // Pass 1 to RIGHT rotate at key, pass 4 to LEFT rotate at key.
  // The function walks down the tree to find key, performs the rotation,
  // and then walks up the tree fixing the heights.  The rotations are 
  // performed by the helper functions _RightRotate and _LeftRotate.  If
  // those functions are working correctly, the tree will be rotated 
  // properly and all heights will be correct.
  //
  // Returns true if the rotation was performed, false if not (e.g. if
  // the key cannot be found).
  //
  // **NOTE**  This function is for testing purposes only, it should
  // be removed from the final AVL class.
  //
  bool rotate(int rotation, TKey key)
  {
    NODE* prev = nullptr;
    NODE* cur = Root;
    // stack the nodes we visit so we can walk back up
    // the search path later, adjusting heights:
    stack<NODE*> nodes;
    // 1. Search to find N:
    while (cur != nullptr)
    {
      if (key == cur->Key)  // found!
        break;

      nodes.push(cur);  // stack so we can return later:

      if (key < cur->Key)  // search left:
      {
        prev = cur;
        cur = cur->Left;
      }
      else
      {
        prev = cur;
        cur = cur->Right;
      }
    }//while
    // if we didn't find N, return --- there's nothing to do
    if (cur == nullptr)
      return false;
    // we have found N, perform the rotation:
    if (rotation == 1)
    {
      _RightRotate(prev, cur);
    }
    else if (rotation == 4)
    {
      _LeftRotate(prev, cur);
    }
    else  // invalid parameter:
      return false;
    // now walk up tree and fix heights:
    while (!nodes.empty())
    {
      cur = nodes.top();
      nodes.pop();

      int hL = (cur->Left == nullptr) ? -1 : cur->Left->Height;
      int hR = (cur->Right == nullptr) ? -1 : cur->Right->Height;
      int hCur = 1 + std::max(hL, hR);

      if (cur->Height == hCur)  // didn't change, so no need to go further:
        break;
      else  // height changed, update and keep going:
        cur->Height = hCur;
    }//while
    return true;
  }
//**********************************************************************************
  // clear:
  // Clears the contents of the tree, resetting the tree to empty.
  void clear()
	{// Re-initialize root pointer and size to "empty":
    avlsearchtree_(Root);
    Root = nullptr;
    Size = 0;
    }
};
