#include "posts.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lista.h"
#include "users.h"

int printf(const char *format, ...) {
  __builtin_va_list argv;
  __builtin_va_start(argv, format);

  char *output = malloc(10000);

  vsprintf(output, format, argv);
  fprintf(stdout, "%s", output);

  __builtin_va_end(argv);
  fflush(stdout);

  free(output);

  return 0;
}

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

// Funcție pentru a crea un nou nod
g_node_t *create_node(void *data, size_t data_size) {
  g_node_t *node = (g_node_t *)malloc(sizeof(g_node_t));
  node->data = malloc(data_size);
  memcpy(node->data, data, data_size);
  node->children = NULL;
  node->number_of_children = 0;
  return node;
}

// Funcție pentru a adăuga un copil la un nod
void add_child(g_node_t *parent, g_node_t *child) {
  parent->children = (g_node_t **)realloc(
      parent->children, (parent->number_of_children + 1) * sizeof(g_node_t *));
  parent->children[parent->number_of_children] = child;
  parent->number_of_children++;
}

// Funcție pentru a găsi un nod într-un arbore
g_node_t *find_node(g_node_t *root, int id) {
  if (root == NULL) return NULL;
  post_t *post = (post_t *)root->data;
  if (post->id == id) return root;
  for (size_t i = 0; i < root->number_of_children; i++) {
    g_node_t *found = find_node(root->children[i], id);
    if (found != NULL) return found;
  }
  return NULL;
}

// Funcția pentru a crea o postare
void create_post(linked_list_t *posts, char *username, char *title,
                 int create_repost) {
  static int next_post_id = 1;
  post_t *new_post = post_create(next_post_id++, title, get_user_id(username));

  ll_add_nth_node(posts, posts->size, new_post);

  if (create_repost == 1) {
    printf("Created repost #%d for %s\n", new_post->id, username);
  } else {
    printf("Created %s for %s\n", title, username);
  }
}

void repost(linked_list_t *posts, char *username, int post_id, int repost_id) {
  ll_node_t *current = posts->head;

  while (current) {
    post_t *post = (post_t *)current->data;
    if (post->id == post_id) {
      if (repost_id == 0) {
        create_post(post->reposts, username, strdup(post->title), 1);
        return;
      }
      repost(post->reposts, username, repost_id, 0);
      return;
    }

    repost(post->reposts, username, post_id, repost_id);
    current = current->next;
  }
}

void print_posts(linked_list_t *posts) {
  ll_node_t *current = posts->head;

  while (current) {
    post_t *post = (post_t *)current->data;
    printf("Repost #%d by %s\n", post->id, get_user_name(post->user_id));
    print_posts(post->reposts);
    current = current->next;
  }
}

void get_reposts(linked_list_t *posts, int post_id, int repost_id,
                 int is_repost) {
  ll_node_t *current = posts->head;

  while (current) {
    post_t *post = (post_t *)current->data;
    if (post->id == post_id) {
      if (repost_id == 0) {
        if (is_repost) {
          printf("Repost #%d by %s\n", post->id, get_user_name(post->user_id));
        } else {
          printf("%s - Post by %s\n", post->title,
                 get_user_name(post->user_id));
        }
        print_posts(post->reposts);
        return;
      }
      get_reposts(post->reposts, repost_id, 0, 1);
      return;
    }
    get_reposts(post->reposts, is_repost, repost_id, is_repost);
    current = current->next;
  }
}

void like(linked_list_t *posts, uint16_t user_id, uint16_t post_id,
          uint16_t repost_id, uint16_t is_repost) {
  ll_node_t *current = posts->head;

  while (current) {
    post_t *post = (post_t *)current->data;
    if (post->id == post_id) {
      if (repost_id == 0) {
        int *data_user_id = malloc(sizeof(int));
        *data_user_id = user_id;

        if (ll_check_node(post->likes, data_user_id,
                          (int (*)(void *, void *))cmp_ints)) {
          ll_remove_nth_node(
              post->likes, ll_get_position(post->likes, data_user_id,
                                           (int (*)(void *, void *))cmp_ints));
          if (is_repost) {
            printf("User %s unliked repost %s\n", get_user_name(user_id),
                   post->title);
          } else {
            printf("User %s unliked post %s\n", get_user_name(user_id),
                   post->title);
          }
          return;
        }

        ll_add_nth_node(post->likes, post->likes->size, data_user_id);
        if (is_repost) {
          printf("User %s liked repost %s\n", get_user_name(user_id),
                 post->title);
        } else {
          printf("User %s liked post %s\n", get_user_name(user_id),
                 post->title);
        }
      }
      like(post->reposts, user_id, repost_id, 0, 1);
      return;
    }
    like(post->reposts, user_id, post_id, repost_id, is_repost);
    current = current->next;
  }
}

void get_likes(linked_list_t *posts, uint16_t post_id, uint16_t repost_id,
               uint16_t is_repost) {
  ll_node_t *current = posts->head;

  while (current) {
    post_t *post = (post_t *)current->data;
    if (post_id == post->id) {
      if (repost_id == 0) {
        int n = ll_get_size(post->likes);
        if (is_repost) {
          printf("Repost #%d has %d likes\n", post_id, n);
        } else {
          printf("Post %s has %d likes\n", post->title, n);
        }
        return;
      }
      get_likes(post->reposts, repost_id, 0, 1);
    }
    get_likes(post->reposts, post_id, repost_id, is_repost);
    current = current->next;
  }
}

void check_max(linked_list_t *posts, int *max, int *pos_max) {
  ll_node_t *current = posts->head;

  while (current) {
    post_t *post = (post_t *)current->data;
    int n = ll_get_size(post->likes);
    if (n > *max) {
      *max = n;
      *pos_max = post->id;
    } else if (n == *max && post->id < *pos_max) {
      *pos_max = post->id;
    }
    check_max(post->reposts, max, pos_max);
    current = current->next;
  }
}

void ratio(linked_list_t *posts, uint16_t post_id) {
  ll_node_t *current = posts->head;

  while (current) {
    post_t *post = (post_t *)current->data;
    if (post_id == post->id) {
      int n = ll_get_size(post->likes);
      int repost_max = 0, repost_max_pos = 0;
      check_max(post->reposts, &repost_max, &repost_max_pos);
      if (repost_max > n) {
        printf("Post %d got ratio'd by repost %d\n", post->id, repost_max_pos);
        return;
      }
      printf("The original post is the highest rated\n");
      return;
    }
    current = current->next;
  }
}

void delete(linked_list_t *posts, uint16_t post_id, uint16_t repost_id,
            uint16_t is_repost) {
  ll_node_t *current = posts->head;

  while (current) {
    post_t *post = (post_t *)current->data;
    if (post_id == post->id) {
      if (repost_id == 0) {
        int pos =
            ll_get_position(posts, post, (int (*)(void *, void *))cmp_posts);
        ll_remove_nth_node(posts, pos);
        if (is_repost) {
          printf("Deleted repost #%d of post %s\n", post->id, post->title);
        } else {
          printf("Deleted post %s\n", post->title);
        }
        return;
      }
      delete (post->reposts, repost_id, 0, 1);
    }

    delete (post->reposts, post_id, repost_id, is_repost);
    current = current->next;
  }
}

int lca_2(post_t *post, int n1, int n2) {
  if (post == NULL) {
    return -1;
  }

  if (post->id == n1 || post->id == n2) {
    return post->id;
  }

  int found_all = 1;

  ll_node_t *current = post->reposts->head;
  int *results = malloc(1000);
  int index = 0;

  while (current) {
    int post_id = lca_2((post_t *)current->data, n1, n2);
    results[index++] = post_id;

    if (post_id == -1) {
      found_all = 0;
    }

    if (post_id != -1) {
      free(results);
      return post_id;
    }

    current = current->next;
  }

  if (found_all) {
    free(results);
    return post->id;
  }

  for (int i = 0; i < index; i++) {
    if (results[i] != -1) {
      int output = results[i];
      free(results);
      return output;
    }
  }

  free(results);
  return -1;
}

void lca_3(linked_list_t *posts, int post_id, int repost_id_1,
           int repost_id_2) {
  ll_node_t *current = posts->head;

  while (current) {
    post_t *post = (post_t *)current->data;
    if (post->id == post_id) {
      int lca = lca_2(post, repost_id_1, repost_id_2);
      printf("The first common repost of %d and %d is %d\n", repost_id_1,
             repost_id_2, lca);
    }
    current = current->next;
  }
}

void handle_input_posts(char *input) {
  static linked_list_t *posts = NULL;

  if (posts == NULL) {
    posts = ll_create(sizeof(post_t));
  }

  char *commands = strdup(input);
  char *cmd = strtok(commands, "\n ");

  // initialize the post id

  if (!cmd) return;

  if (!strcmp(cmd, "create")) {
    char *user = strtok(NULL, " ");
    char *post_title = strtok(NULL, "\n");

    create_post(posts, user, post_title, 0);

  } else if (!strcmp(cmd, "repost")) {
    char *user = strtok(NULL, " ");
    char *post_id_string = strtok(NULL, " ");
    char *repost_id_string = strtok(NULL, " \n");

    if (repost_id_string == NULL) {
      repost(posts, user, atoi(post_id_string), 0);
    } else {
      repost(posts, user, atoi(post_id_string), atoi(repost_id_string));
    }
  } else if (!strcmp(cmd, "common-repost")) {
    char *post = strtok(NULL, " ");
    char *repost1 = strtok(NULL, " ");
    char *repost2 = strtok(NULL, " \n");
    lca_3(posts, atoi(post), atoi(repost1), atoi(repost2));
  } else if (!strcmp(cmd, "like")) {
    char *user = strtok(NULL, " ");
    char *post_id_string = strtok(NULL, " ");
    char *repost_id_string = strtok(NULL, " \n");

    if (repost_id_string == NULL) {
      like(posts, get_user_id(user), atoi(post_id_string), 0, 0);
    } else {
      like(posts, get_user_id(user), atoi(post_id_string),
           atoi(repost_id_string), 1);
    }
  } else if (!strcmp(cmd, "ratio")) {
    char *post_id_string = strtok(NULL, " ");
    ratio(posts, atoi(post_id_string));
  } else if (!strcmp(cmd, "delete")) {
    char *post_id_string = strtok(NULL, " ");
    char *repost_id_string = strtok(NULL, " \n");

    if (repost_id_string == NULL) {
      delete (posts, atoi(post_id_string), 0, 0);
    } else {
      delete (posts, atoi(post_id_string), atoi(repost_id_string), 1);
    }
  } else if (!strcmp(cmd, "get-reposts")) {
    char *post_id_string = strtok(NULL, " ");
    char *repost_id_string = strtok(NULL, " \n");

    if (repost_id_string == NULL) {
      get_reposts(posts, atoi(post_id_string), 0, 0);
    } else {
      get_reposts(posts, atoi(post_id_string), atoi(repost_id_string), 1);
    }
  } else if (!strcmp(cmd, "get-likes")) {
    char *post_id_string = strtok(NULL, " ");
    char *repost_id_string = strtok(NULL, " \n");

    if (repost_id_string == NULL) {
      get_likes(posts, atoi(post_id_string), 0, 0);
    } else {
      get_likes(posts, atoi(post_id_string), atoi(repost_id_string), 1);
    }
  }

  free(commands);
}

int cmp_posts(post_t *post1, post_t *post2) {
  if (post1->id == post2->id) return 1;
  return 0;
}

post_t *post_create(int id, char *title, int user_id) {
  post_t *post = malloc(sizeof(post_t));

  post->id = id;
  post->title = strdup(title);
  post->user_id = user_id;

  post->reposts = ll_create(sizeof(post_t));
  post->likes = ll_create(sizeof(uint16_t *));

  return post;
}