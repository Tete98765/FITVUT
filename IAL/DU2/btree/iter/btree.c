/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
  *tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  //prepisat
  if (tree == NULL) return false;

  while (tree != NULL) {
    if (key > tree -> value) {
      tree = tree -> right;
    }
    else if (key < tree -> value) {
      tree = tree -> left;
    }
    else {
      *value = tree -> value;
      return true;
    }
  }
  
  return false;

  /*if (tree == NULL) return false;

  while (tree != NULL) {
    if (key < tree -> key) {
      tree = tree -> left;
    }
    else if (key > tree -> key) {
      tree = tree -> right;
    }
    else {
      *value = tree -> value;
      return true;
    }
  }
  
  return false;*/
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  /*bst_node_t *new = (bst_node_t *) malloc(sizeof(struct bst_node));
  if (new == NULL) return; 

  if ((*tree) == NULL) {                                                                                                                                                                                                               
    new -> key = key;
    new -> value = value;
    new -> right = NULL;
    new -> left = NULL;                                                          
    (*tree) = new;                                                               
    return;                                                                        
  }

  bst_node_t *curr = (*tree);

  while (curr != NULL) {                                                                                                                                                 
    if (curr->key == key) {                                                                                                                              
      curr->value = value;                                                        
      return;                                                                      
    }                                                                           
    if ((!curr->left && curr->key > key) || (!curr->right && curr->key < key)) {
                                                                                                                                                  
      new -> key = key;
      new -> value = value;
      new -> right = NULL;
      new -> left = NULL;                                                           
      if (curr -> key > key) curr->left = new;                                                                                                               
      else curr->right = new;                                                                          
                                                             
      return;  
    }                                                                    
                                                                               
    if (curr->key > key)                                                          
      curr = curr->left;                                                         
    else                                                                        
      curr = curr->right;
  }       */                                         
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  bst_node_t *right_tree = (*tree);
  bst_node_t *left_tree = NULL; 

  while(right_tree -> right != NULL) {
    left_tree = right_tree;
    right_tree = right_tree -> right;
  }

  target -> value = right_tree -> value;
  target -> key = right_tree -> key;
    
  if(right_tree -> left == NULL) left_tree -> right = NULL;
  else left_tree -> right = right_tree->left;
      
  free(right_tree);
  right_tree = NULL;
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
  bst_node_t *curr = (*tree);                      
  bst_node_t *prev = NULL;                       

  if (curr == NULL) return;
  else {  
    while (curr != NULL && curr -> value != key) {                                    
                                                 
    if (curr->key > key){                                                                    
      prev = curr;                                  
      curr = curr->left;                           
    }                                              
    else if (curr->key < key) {                                                          
      prev = curr;                                  
      curr = curr->right;                          
    }                                              
    else if (curr->right == NULL && curr->left == NULL) {                                                 
      if (prev -> right == curr)                      
        prev -> right = NULL;                         
      else                                         
        prev -> left = NULL;                          
        free(curr); 
        curr = NULL;                                                                      
    }                                              
    else if (curr -> left != NULL && curr -> right != NULL) bst_replace_by_rightmost(curr, &curr->left);            
    else if(curr -> right != NULL && curr -> left == NULL) {
      if(prev -> key == curr -> key){
        prev -> left = curr -> right;
      }
      else {
        prev -> right = curr -> right;
      }
      free(curr);
      curr = NULL;
    }
    else if(curr -> right == NULL && curr -> left != NULL) {
      if(prev -> key > curr -> key) {
        prev -> left = curr -> left;
      }
      else if(prev -> key < curr -> key){
        prev -> right = curr -> left;
      }
      free(curr);
      curr = NULL;
      }
    }
  }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {

  stack_bst_t stack;
  stack_bst_init(&stack);
  while ((*tree) != NULL || !stack_bst_empty(&stack)) {
    if ((*tree) == NULL) {
      if (!stack_bst_empty(&stack)) {
        *tree = stack_bst_top(&stack);
        stack_bst_pop(&stack);
      }
    }
    else {
      if ((*tree) -> right != NULL) stack_bst_push(&stack, (*tree)->right);
      bst_node_t *delete = *tree;
      *tree = (*tree)->left;
		  free(delete);
    }
  } 
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {

  while (tree != NULL) {	
		stack_bst_push(to_visit, tree);
    bst_print_node(tree);
    tree = tree->left;
	}
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
  stack_bst_t stack;
  stack_bst_init(&stack);

  bst_leftmost_preorder(tree, &stack);

  while (!stack_bst_empty(&stack)) {
    tree = stack_bst_top(&stack);
    stack_bst_pop(&stack);
    bst_leftmost_preorder(tree->right, &stack);
  }
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
  while(tree != NULL) {
    stack_bst_push(to_visit, tree);
    tree = tree -> left;
  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
  stack_bst_t stack;
  stack_bst_init(&stack);

  bst_leftmost_inorder(tree, &stack);

  while (!stack_bst_empty(&stack)) {
    tree = stack_bst_top(&stack);
    stack_bst_pop(&stack);
    bst_print_node(tree);
    bst_leftmost_inorder(tree->right, &stack);
  }
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit, stack_bool_t *first_visit) {
  
  while (tree != NULL) {
    stack_bst_push(to_visit, tree);
    stack_bool_push(first_visit, true);
    tree = tree->left;
  }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {

  bool tmp_bool;
  stack_bst_t stack;
  stack_bst_init(&stack);
  
  stack_bool_t bool_stack;
  stack_bool_init(&bool_stack);

  bst_leftmost_postorder(tree, &stack, &bool_stack);

  while (!stack_bst_empty(&stack)) {
    tree = stack_bst_top(&stack);
    tmp_bool = stack_bool_top(&bool_stack);
    stack_bst_pop(&stack);
    stack_bool_pop(&bool_stack);

    if (tmp_bool) {
      stack_bool_push(&bool_stack, false);
      stack_bst_push(&stack, tree);

      bst_leftmost_postorder(tree->right, &stack, &bool_stack);
    }
    else bst_print_node(tree);
  }
}
