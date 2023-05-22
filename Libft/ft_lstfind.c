#include "libft.h"
#include <stdbool.h>

t_list	*ft_lstfind(t_list *start, bool (*f)(void *)) {
	while (start != NULL) {
		if (f(start->content))
			return (start);
		start = start->next;
	}
	return (NULL);
}

t_list	*ft_lstfind_l(t_list *start, void *data, bool (*f)(t_list *, void *)) {
	while (start != NULL) {
		if (f(start, data))
			return (start);
		start = start->next;
	}
	return (NULL);
}

t_list	**ft_lstfind_pl(t_list **start, void *data, bool (*f)(t_list *, void *)) {
	while (*start != NULL) {
		if (f(*start, data))
			return (start);
		start = &((*start)->next);
	}
	return (NULL);
}
