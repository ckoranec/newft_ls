#include "../includes/ft_ls.h"

int     sortbyflag(char flags, t_node *a, t_node *b)
{
	if (flags)
		;
	if (flags & 0x1)
		return (ft_strcmp(a->name, b->name) > 0 ? 1 : 0);
    else if (ft_strcmp(a->name, b->name) < 0)
        return (1);
    return (0);
}

t_node		*sortedmerge_list(t_node *a, t_node *b, char flags)
{
	t_node	*result;

	result = NULL;
	if (a == NULL)
		return (b);
	else if (b == NULL)
		return (a);
	if (sortbyflag(flags, a, b))
	{
		result = a;
		result->next = sortedmerge_list(a->next, b, flags);
	}
	else
	{
		result = b;
		result->next = sortedmerge_list(a, b->next, flags);
	}
	return (result);
}

void		fbsplit_list(t_node *source, t_node **frontref, t_node **backref)
{
	t_node	*fast;
	t_node	*slow;

	slow = source;
	fast = source->next;
	while (fast != NULL)
	{
		fast = fast->next;
		if (fast != NULL)
		{
			slow = slow->next;
			fast = fast->next;
		}
	}
	*frontref = source;
	*backref = slow->next;
	slow->next = NULL;
}

t_node		**mergesort_list(t_node **list, char flags)
{
	t_node	*head;
	t_node	*a;
	t_node	*b;

	head = *list;
	if ((head == NULL) || (head->next == NULL))
		return (list);
	fbsplit_list(head, &a, &b);
	mergesort_list(&a, flags);
	mergesort_list(&b, flags);
	*list = sortedmerge_list(a, b, flags);
	return (list);
}