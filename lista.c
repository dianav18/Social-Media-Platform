#include "lista.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

linked_list_t* ll_create(int data_size) {
  /* TODO */
  linked_list_t* list = malloc(sizeof(*list));
  list->head = NULL;
  list->data_size = data_size;
  list->size = 0;
  return list;
}

/*
 * Pe baza datelor trimise prin pointerul new_data, se creeaza un nou nod care e
 * adaugat pe pozitia n a listei reprezentata de pointerul list. Pozitiile din
 * lista sunt indexate incepand cu 0 (i.e. primul nod din lista se afla pe
 * pozitia n=0). Daca n >= nr_noduri, noul nod se adauga la finalul listei. Daca
 * n < 0, ignorati comanda (iesiti din functie).
 */
void ll_add_nth_node(linked_list_t* list, int n,
                     const void* new_data) {
  /* TODO */

  if (n < 0) return;

  ll_node_t* new_node = malloc(sizeof(*new_node));
  new_node->data = malloc(list->data_size);
  memcpy(new_node->data, new_data, list->data_size);
  ll_node_t* current = list->head;

  if (n == 0 || list->size == 0) {
    new_node->next = list->head;
    list->head = new_node;
    list->size++;
    return;
  }

  if (n >= list->size) {
    for (int i = 0; i < list->size - 1 && current->next != NULL; i++) {
      current = current->next;
    }
    current->next = new_node;
    new_node->next = NULL;
    list->size++;
    return;
  }

  for (int i = 0; i < n - 1 && current->next != NULL; i++) {
    current = current->next;
  }
  new_node->next = current->next;
  current->next = new_node;
  list->size++;
}

/*
 * Elimina nodul de pe pozitia n din lista al carei pointer este trimis ca
 * parametru. Pozitiile din lista se indexeaza de la 0 (i.e. primul nod din
 * lista se afla pe pozitia n=0). Daca n >= nr_noduri - 1, se elimina nodul de
 * la finalul listei. Daca n < 0, ignorati comanda (iesiti din functie).
 * Functia intoarce un pointer spre acest nod proaspat eliminat din lista.
 * Este responsabilitatea apelantului sa elibereze memoria acestui nod.
 */
ll_node_t* ll_remove_nth_node(linked_list_t* list, int n) {
  if (list->head == NULL) {
    return NULL;
  }

  ll_node_t* removed_node = NULL;

  if (n == 0) {
    removed_node = list->head;
    list->head = list->head->next;
  } else if (n >= list->size - 1) {
    ll_node_t* current = list->head;
    for (int i = 0; i < list->size - 2 && current->next != NULL; i++) {
      current = current->next;
    }
    removed_node = current->next;
    current->next = NULL;
  } else {
    ll_node_t* current = list->head;
    for (int i = 0; i < n - 1 && current->next != NULL; i++) {
      current = current->next;
    }

    if (current->next != NULL) {
      removed_node = current->next;
      current->next = current->next->next;
    }
  }

  if (removed_node != NULL) {
    list->size--;
  }

  return removed_node;
}

/*
 * Functia intoarce numarul de noduri din lista al carei pointer este trimis ca
 * parametru.
 */
int ll_get_size(linked_list_t* list) { /* TODO */ return list->size; }

/*
 * Procedura elibereaza memoria folosita de toate nodurile din lista, iar la
 * sfarsit, elibereaza memoria folosita de structura lista si actualizeaza la
 * NULL valoarea pointerului la care pointeaza argumentul (argumentul este un
 * pointer la un pointer).
 */
void ll_free(linked_list_t** pp_list) { /* TODO */

  ll_node_t* current = (*pp_list)->head;
  ll_node_t* next;
  while (current != NULL) {
    next = current->next;
    free(current->data);
    free(current);
    current = next;
  }
  free(*pp_list);
  *pp_list = NULL;
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza int-uri. Functia afiseaza toate valorile int stocate in nodurile
 * din lista inlantuita separate printr-un spatiu.
 */
void ll_print_int(linked_list_t* list) {
  /* TODO */
  ll_node_t* current = list->head;
  while (current != NULL) {
    printf("%d ", *(int*)current->data);
    current = current->next;
  }
  printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza string-uri. Functia afiseaza toate string-urile stocate in
 * nodurile din lista inlantuita, separate printr-un spatiu.
 */
void ll_print_string(linked_list_t* list) {
  /* TODO */
  ll_node_t* current = list->head;
  while (current != NULL) {
    printf("%s ", (char*)current->data);
    current = current->next;
  }
  printf("\n");
}