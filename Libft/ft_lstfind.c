#include "libft.h"

t_list	*ft_lstfind(t_list *start, bool (*f)(void *)) {
	while (start != NULL) {
		if (f(start->content))
			return (start);
		start = start->next;
	}
	return (NULL);
}