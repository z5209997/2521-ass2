// 1. List_of_Urls <- GetCollection(	)
// Create	a	set	(list)	of	urls to	process	by	reading	data	from	file “collection.txt”

// 2. Graph	 g	<- GetGraph(List_of_Urls )
// Create	empty	graph	(use	graph	ADT	in	say	graph.h and	graph.c)
// For	each	url in	the	above	list	
// • read	<url>.txt	file,	and	update	graph	by	adding	a	node	and	outgoing links	

// 3. InvertedList <- GetInvertedList(List_of_Urls )
// Create	empty	inverted	list	(use	say	List	of	lists,	BST	where	values	are	lists,	etc)
// For	each	url in	List_of_Urls
// • read	<url>.txt	file,	and	update	inverted	index		