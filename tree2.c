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

g_tree_t *g_tree_create(size_t data_size) {
  g_tree_t *g_tree;

  g_tree = malloc(sizeof(*g_tree));
  DIE(g_tree == NULL, "g_tree malloc");

  g_tree->root = NULL;
  g_tree->data_size = data_size;

  return g_tree;
}

// add a node to the tree
static g_node_t *__g_tree_add_node(g_node_t *node, void *data,
                                   size_t data_size) {
  g_node_t *new_node;

  if (node == NULL) {
    new_node = __g_node_create(data, data_size);
    return new_node;
  }

  node->children = realloc(node->children,
                           (node->number_of_children + 1) * sizeof(g_node_t *));
  DIE(node->children == NULL, "node->children realloc");

  new_node = __g_node_create(data, data_size);
  node->children[node->number_of_children] = new_node;
  node->number_of_children++;

  return node;
}

// find a node in the tree
static g_node_t *__g_tree_find_node(g_node_t *node, void *data,
                                    size_t data_size) {
  size_t i;

  if (memcmp(node->data, data, data_size) == 0) {
    return node;
  }

  for (i = 0; i < node->number_of_children; i++) {
    g_node_t *found_node =
        __g_tree_find_node(node->children[i], data, data_size);
    if (found_node != NULL) {
      return found_node;
    }
  }

  return NULL;
}

// print the tree
static void __g_tree_print(g_node_t *node, int level) {
  size_t i;

  for (i = 0; i < level; i++) {
    printf("  ");
  }

  printf("%s\n", (char *)node->data);

  for (i = 0; i < node->number_of_children; i++) {
    __g_tree_print(node->children[i], level + 1);
  }
}

// free the tree
static void __g_tree_free(g_node_t *node) {
  size_t i;

  for (i = 0; i < node->number_of_children; i++) {
    __g_tree_free(node->children[i]);
  }

  free(node->children);
  free(node->data);
  free(node);
}

// read the tree
void read_tree(g_tree_t *g_tree) {
  int i, N;
  char buf[BUF_SIZ];
  char *token;
  const char delim[2] = " ";

  // Read the number of lines (N)
  fgets(buf, BUF_SIZ, stdin);
  sscanf(buf, "%d\n", &N);

  // Read each line containing node relationships
  for (i = 0; i < N; ++i) {
    fgets(buf, BUF_SIZ, stdin);
    token = strtok(buf, delim);

    // Read the parent node
    char parent_data[BUF_SIZ];
    strcpy(parent_data, token);

    g_node_t *parent_node;
    if (i == 0) {
      g_tree->root =
          __g_tree_add_node(g_tree->root, parent_data, strlen(parent_data) + 1);
      parent_node = g_tree->root;
    } else {
      parent_node = __g_tree_find_node(g_tree->root, parent_data,
                                       strlen(parent_data) + 1);
      DIE(parent_node == NULL, "Parent node not found");
    }

    // Read the child nodes
    while ((token = strtok(NULL, delim)) != NULL) {
      __g_tree_add_node(parent_node, token, strlen(token) + 1);
    }
  }
}

int main(void) {
  g_tree_t *generic_tree;

  generic_tree = g_tree_create(sizeof(char) * BUF_SIZ);

  read_tree(generic_tree);

  __g_tree_print(generic_tree->root, 0);

  __g_tree_free(generic_tree->root);
  free(generic_tree);

  return 0;
}
