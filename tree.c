#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 500
#define BUF_SIZ 512

// generic tree implementation

#define DIE(assertion, call_description)                 \
  do {                                                   \
    if (assertion) {                                     \
      fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__); \
      perror(call_description);                          \
      exit(errno);                                       \
    }                                                    \
  } while (0)

typedef struct g_node_t g_node_t;
struct g_node_t {
  /* children */
  g_node_t **children;
  size_t number_of_children;

  /* data contained by the node */
  void *data;
};

typedef struct g_tree_t g_tree_t;
struct g_tree_t {
  /* root of the tree */
  g_node_t *root;

  /* size of the data contained by the nodes */
  size_t data_size;
};

/* Helper queue data structure definitions */
typedef struct queue_t queue_t;
queue_t *q_create(unsigned int data_size, unsigned int max_size);
unsigned int q_get_size(queue_t *q);
unsigned int q_is_empty(queue_t *q);
void *q_front(queue_t *q);
int q_dequeue(queue_t *q);
int q_enqueue(queue_t *q, void *new_data);
void q_clear(queue_t *q);
void q_free(queue_t *q);

// create a node
static g_node_t *__g_node_create(void *data, size_t data_size) {
  g_node_t *g_node;

  g_node = malloc(sizeof(*g_node));
  DIE(g_node == NULL, "g_node malloc");

  g_node->children = NULL;
  g_node->number_of_children = 0;
  g_node->data = malloc(data_size);
  DIE(g_node->data == NULL, "g_node->data malloc");
  memcpy(g_node->data, data, data_size);

  return g_node;
}

// create a tree
g_tree_t *g_tree_create(size_t data_size) {
  g_tree_t *g_tree = malloc(sizeof(*g_tree));
  DIE(g_tree == NULL, "g_tree malloc");

  g_tree->root = NULL;
  g_tree->data_size = data_size;

  return g_tree;
}

// add a node to the tree
void g_tree_add_node(g_tree_t *g_tree, void *data, size_t data_size,
                     g_node_t *parent) {
  g_node_t *g_node = __g_node_create(data, data_size);

  if (parent == NULL) {
    g_tree->root = g_node;
  } else {
    parent->children =
        realloc(parent->children,
                (parent->number_of_children + 1) * sizeof(*parent->children));
    DIE(parent->children == NULL, "realloc parent->children");

    parent->children[parent->number_of_children] = g_node;
    parent->number_of_children++;
  }
}

// destroy a node
static void __g_node_destroy(g_node_t *g_node) {
  size_t i;

  for (i = 0; i < g_node->number_of_children; ++i) {
    __g_node_destroy(g_node->children[i]);
  }

  free(g_node->children);
  free(g_node->data);
  free(g_node);
}

// destroy a tree
void g_tree_destroy(g_tree_t *g_tree) {
  if (g_tree->root != NULL) {
    __g_node_destroy(g_tree->root);
  }

  free(g_tree);
}

// cauta un nod in arbore
g_node_t *g_tree_find_node(g_node_t *g_node, void *data, size_t data_size) {
  size_t i;
  g_node_t *found = NULL;

  if (memcmp(g_node->data, data, data_size) == 0) {
    return g_node;
  }

  for (i = 0; i < g_node->number_of_children; ++i) {
    found = g_tree_find_node(g_node->children[i], data, data_size);
    if (found != NULL) {
      return found;
    }
  }

  return NULL;
}

// afiseaza un arbore
void g_tree_print(g_node_t *g_node, size_t level) {
  size_t i, j;

  for (i = 0; i < level; ++i) {
    printf("  ");
  }

  printf("%s\n", (char *)g_node->data);

  for (i = 0; i < g_node->number_of_children; ++i) {
    g_tree_print(g_node->children[i], level + 1);
  }
}

// citeste un arbore
void read_tree(g_tree_t *g_tree) {
  int i, N;
  char buf[BUF_SIZ];
  char *token;
  const char delim[2] = " ";

  fgets(buf, BUF_SIZ, stdin);
  sscanf(buf, "%d\n", &N);

  for (i = 0; i < N; ++i) {
    fgets(buf, BUF_SIZ, stdin);
    token = strtok(buf, delim);
    g_tree_add_node(g_tree, token, strlen(token) + 1, NULL);
  }
}

//citeste un arbore si afiseaza arborele
int main() { 
    g_tree_t *generic_tree;
    generic_tree = g_tree_create(sizeof(char) * BUF_SIZ);
    read_tree(generic_tree);
    g_tree_print(generic_tree->root, 5);
    g_tree_destroy(generic_tree);
    return 0;
}

struct queue_t {
  /* Dimensiunea maxima a cozii */
  unsigned int max_size;
  /* Dimensiunea cozii */
  unsigned int size;
  /* Dimensiunea in octeti a tipului de date stocat in coada */
  unsigned int data_size;
  /* Indexul de la care se vor efectua operatiile de front si dequeue */
  unsigned int read_idx;
  /* Indexul de la care se vor efectua operatiile de enqueue */
  unsigned int write_idx;
  /* Bufferul ce stocheaza elementele cozii */
  void **buff;
};

queue_t *q_create(unsigned int data_size, unsigned int max_size) {
  queue_t *q = calloc(1, sizeof(*q));
  DIE(!q, "calloc queue failed");

  q->data_size = data_size;
  q->max_size = max_size;

  q->buff = malloc(max_size * sizeof(*q->buff));
  DIE(!q->buff, "malloc buffer failed");

  return q;
}

unsigned int q_get_size(queue_t *q) { return !q ? 0 : q->size; }

unsigned int q_is_empty(queue_t *q) { return !q ? 1 : !q->size; }

void *q_front(queue_t *q) {
  if (!q || !q->size) return NULL;

  return q->buff[q->read_idx];
}

int q_dequeue(queue_t *q) {
  if (!q || !q->size) return 0;

  free(q->buff[q->read_idx]);

  q->read_idx = (q->read_idx + 1) % q->max_size;
  --q->size;
  return 1;
}

int q_enqueue(queue_t *q, void *new_data) {
  void *data;
  if (!q || q->size == q->max_size) return 0;

  data = malloc(q->data_size);
  DIE(!data, "malloc data failed");
  memcpy(data, new_data, q->data_size);

  q->buff[q->write_idx] = data;
  q->write_idx = (q->write_idx + 1) % q->max_size;
  ++q->size;

  return 1;
}

void q_clear(queue_t *q) {
  unsigned int i;
  if (!q || !q->size) return;

  for (i = q->read_idx; i != q->write_idx; i = (i + 1) % q->max_size)
    free(q->buff[i]);

  q->read_idx = 0;
  q->write_idx = 0;
  q->size = 0;
}

void q_free(queue_t *q) {
  if (!q) return;

  q_clear(q);
  free(q->buff);
  free(q);
}