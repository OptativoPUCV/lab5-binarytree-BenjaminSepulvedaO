#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (tree == NULL || tree->root == NULL)
    {
        tree->root = createTreeNode(key, value);
        return;
    }
    TreeNode *aux = tree->root;
    while (aux != NULL)
    {
        if (is_equal(tree, key, aux->pair->key)) return;
        if (tree->lower_than(key, aux->pair->key) == 1)
        {
            if (aux->left == NULL)
            {
                aux->left = createTreeNode(key, value);
                aux->left->parent = aux;
                tree->current = aux->left;
                return;
            }
            else aux = aux->left;
        }
        else
        {
            if (aux->right == NULL)
            {
                aux->right = createTreeNode(key, value);
                aux->right->parent = aux;
                tree->current = aux->right;
                return;
            }
            else aux = aux->right;
        }
    }
}

TreeNode * minimum(TreeNode * x){
    if (x == NULL) return NULL;
    if (x->left == NULL) return x;
    return minimum(x->left);
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if (node == NULL) return;
    if (node->left == NULL && node->right == NULL)
    {
        if (node->parent == NULL)
        {
            tree->root = NULL;
            free(node);
            return;
        }
        if (node->parent->left == node) node->parent->left = NULL;
        else node->parent->right = NULL;
    }
    else if (node->left == NULL || node->right == NULL)
    {
        TreeNode child;
        if (node->left != NULL) child = *node->left;
        else child = *node->right;
        child.parent = node->parent;
        if (node->parent == NULL)
        {
            tree->root = &child;
            free(node);
            return;
        }
        if (node->parent->left == node) node->parent->left = &child;
        else node->parent->right = &child;
    }
    else
    {
        TreeNode *min = minimum(node->right);
        node->pair->key = min->pair->key;
        node->pair->value = min->pair->value;
        removeNode(tree, min);
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode *aux = tree->root;
    if (aux == NULL) return NULL;
    if (is_equal(tree, aux->pair->key, key))
    {
        tree->current = aux;
        return aux->pair;
    } 
    while (aux != NULL)
    {
        if (tree->lower_than(key, aux->pair->key) == 1)
        {
            aux = aux->left;
        }
        else if (is_equal(tree, aux->pair->key, key))
        {
            tree->current = aux;
            return aux->pair;
        }
        else
        {
            aux = aux->right;
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    TreeNode *aux = tree->root;
    TreeNode *ub_node = NULL;
    if (aux == NULL) return NULL;
    if (is_equal(tree, aux->pair->key, key)) return aux->pair;
    while (aux != NULL)
    {
        if (tree->lower_than(key, aux->pair->key) == 1)
        {
            ub_node = aux;
            aux = aux->left;
        }
        else if (is_equal(tree, aux->pair->key, key))
        {
            ub_node = aux;
            aux = aux->right;
        }
        else
        {
            aux = aux->right;
        }
    }
    if (ub_node == NULL) return NULL;
    return ub_node->pair;
}

Pair * firstTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode *aux = tree->root;
    while (aux->left != NULL)
    {
        aux = aux->left;
    }
    return aux->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode *aux = tree->current;
    if (aux->right != NULL)
    {
        aux = aux->right;
        while (aux->left != NULL)
        {
            aux = aux->left;
        }
        tree->current = aux;
        return aux->pair;
    }
    else
    {
        TreeNode *aux2 = aux->parent;
        while (aux2 != NULL && aux == aux2->right)
        {
            aux = aux2;
            aux2 = aux2->parent;
        }
        if (aux2 != NULL)
        {
            tree->current = aux2;
            return aux2->pair;
        }
        else return NULL;
    }
}
