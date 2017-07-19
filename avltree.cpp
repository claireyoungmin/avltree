#include <iostream>
#include <cmath>

using namespace std;

template <class T>
class avlnode{
public:
	T item;
	avlnode<T> *parent;
	avlnode<T> *left;
	avlnode<T> *right;
	int height;
	int balance;
	int child;
	avlnode();
	avlnode(T);
};

template <class T>
avlnode<T>::avlnode(){
	parent = NULL;
    left = NULL;
    right = NULL;
    height = 0;
    balance = 0;
    child = 0;
};

template <class T>
avlnode<T>::avlnode(T x){
	parent = NULL;
    left = NULL;
    right = NULL;
    height = 0;
    balance = 0;
    child = 0;
	item = x;
}

template <class T>
class avl{
private:
	avlnode<T> *root;
	avlnode<T>* min(avlnode<T>*);
	void leftRotate(avlnode<T>*);
	void rightRotate(avlnode<T>*);
	int height(avlnode<T>*);
	int balance(avlnode<T>*);
public:
	avl();
	~avl();
	int search(T);
	void insert(T);
	int remove(T);
};

template <class T>
avl<T>::avl(){
	root = NULL;
}

template <class T>
avl<T>::~avl(){
	while(root!=NULL)
		remove(root->item);
}
template <class T>
int avl<T>::search(T x){
	avlnode<T> *r = root;
	int count = 0;
	while(r!=NULL){
		count++;
		if(x == r->item) return count+1;
		else if(x < r->item)r = r->left;
		else if(x >= r->item)r = r->right;
	}
	return count*(-1);
}

template <class T>
void avl<T>::insert(T x){
    avlnode<T> *n = new avlnode<T>(x);
    if(root == NULL)
        root = n;
    else{
        avlnode<T> *r = root,*r2;
        while(r!=NULL){
            r2 = r;
            if(x < r->item) r = r->left;
            else if (x >= r->item) r = r->right;
        }
        n->parent = r2;
        if(x < r2->item){
            n->child = -1;
            r2->left = n;
        }
        else{
        	n->child = 1;
            r2->right = n;
        }
        //Tree is already created.
        for(avlnode<T> *tmp = n; tmp!=NULL; tmp=tmp->parent){
        	tmp->height = 1+max(height(tmp->left),height(tmp->right));
        	if(height(tmp->left)>height(tmp->right)) tmp->balance = -1;
        	else if(height(tmp->left)<height(tmp->right)) tmp->balance = 1;
        	else tmp->balance = 0;

        	if(abs(height(tmp->left)-height(tmp->right))>1){
        		if(balance(tmp)==-1){
        			if(balance(tmp->left)==-1) rightRotate(tmp);  // left leaning case
        			else if(balance(tmp->left)==1){  // left-right leaning case
        				leftRotate(tmp->left);
        				rightRotate(tmp);
        			}
        		}
        		else if(balance(tmp)==1){
        			if(balance(tmp->right)==1) leftRotate(tmp);  // right leaning case
        			else if(balance(tmp->right)==-1){  // right-left leaning case
        				rightRotate(tmp->right);
        				leftRotate(tmp);
        			}
        		}
        	}
        }
    }
}

template <class T>
avlnode<T>* avl<T>::min(avlnode<T>* r){
    avlnode<T> *r2;
	while(r!=NULL){
        r2 = r;
        r = r->left;
    }
    return r2;
}

template <class T>
int avl<T>::remove(T x){
	avlnode<T> *r = root;
	int count = 0;
    while(r!=NULL){
		if(x == r->item){
			if(r->left == NULL && r->right == NULL){  //  leaf case.
				if(r == root)
					root = NULL;
				else if(r->parent->right == r)  // if right child.
					r->parent->right = NULL;
				else
					r->parent->left = NULL;
				r->parent = NULL;
				delete r;
			}
			else if(r->left==NULL || r->right==NULL){  // one child case
				if(r == root){
					if(r->right!=NULL)
						root = r->right;
					else
						root = r->left;
				}
				else if(r->parent->right == r){ // if right child
					r->parent->right = r->right;
					r->right->parent = r->parent;
				}
				else{
					r->parent->left = r->left;
					r->left->parent = r->parent;
				}
				r->left = r->right = r->parent = NULL;
				delete r;
			}
			else if(r->left!=NULL && r->right!=NULL){  // two children case.
                avlnode<T> *rm;  //  rm is the successor.
                if(r->right->left!=NULL) rm = min(r->right);
                else rm = r->right;  // if walay left ang right child ni r after all.
				T minVal = rm->item;
				remove(rm->item);
				r->item = minVal;
			}
			return count+1;
		}
		else if(x < r->item)
		   r = r->left;
		else if(x > r->item)
	       r = r->right;
        count++;
    }
	return (-1)*count;
}


template <class T>
int avl<T>::height(avlnode<T> *n){
	if(n==NULL)
		return -1;
	else
		return n->height;
}

template <class T>
int avl<T>::balance(avlnode<T> *n){
	if(n==NULL)
		return 0;
	else
		return n->balance;
}

template <class T>
void avl<T>::rightRotate(avlnode<T> *n){
	//setting links.
	avlnode<T> *tmp = n->left;
	avlnode<T> *tmp2 = tmp->right;
	n->left = tmp2;
	if(tmp2!=NULL){
		tmp2->parent = n;
		tmp2->child = -1;
	}
	tmp->parent = n->parent;
	if(tmp->parent!=NULL){
		if(n->child<0)
			tmp->parent->left = tmp;
		else
			tmp->parent->right = tmp;
	}
	else
		root = tmp;
	tmp->right = n;
	n->parent = tmp;
	//setting all other stuff.
	tmp->child = n->child;
	n->child = 1;
	n->height = 1+max(height(n->left),height(n->right));
	tmp->height = 1+max(height(tmp->left),height(tmp->right));

	if(height(n->left)>height(n->right)) n->balance = -1;
	else if (height(n->left)<height(n->right)) n->balance = 1;
	else n->balance = 0;

	if(height(tmp->left)>height(tmp->right)) tmp->balance = -1;
	else if (height(tmp->left)<height(tmp->right)) tmp->balance = 1;
	else tmp->balance = 0;
}

template <class T>
void avl<T>::leftRotate(avlnode<T> *n){
	//setting links.
	avlnode<T> *tmp = n->right;
	avlnode<T> *tmp2 = tmp->left;
	n->right = tmp2;
	if(tmp2!=NULL){
		tmp2->parent = n;
		tmp2->child = 1;
	}
	tmp->parent = n->parent;
	if(tmp->parent!=NULL){
		if(n->child<0)
			tmp->parent->left = tmp;
		else
			tmp->parent->right = tmp;
	}
	else
		root = tmp;
	tmp->left = n;
	n->parent = tmp;
	//setting all other stuff.
	tmp->child = n->child;
	n->child = -1;
	n->height = 1+max(height(n->left),height(n->right));
	tmp->height = 1+max(height(tmp->left),height(tmp->right));

	if(height(n->left)>height(n->right)) n->balance = -1;
	else if (height(n->left)<height(n->right)) n->balance = 1;
	else n->balance = 0;

	if(height(tmp->left)>height(tmp->right)) tmp->balance = -1;
	else if (height(tmp->left)<height(tmp->right)) tmp->balance = 1;
	else tmp->balance = 0;
}

int main(){
	int n,m,x,r,y;
	cin >> n;
	for(int i = 0; i < n; i++){
		avl<int> t;
		cin >> m;
		for(int j = 0; j < m; j++) {
			cin >> x;
			t.insert(x);
		}
		cin >> r;
		if((y = t.remove(r))>=0)
            cout << y << " DELETED" <<endl;
        else
            cout << abs(y) << " !FOUND" << endl;
	}
}
