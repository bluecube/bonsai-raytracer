#include "Container.h"
#include "utils.h"

Container::Container(Mode m) : tmpInstances(0), tree(0), rebuilt(true), mode(m), cItems(0) {}

/// Delete the KD tree, all Instance objects,
/// and all Instances in the list of items waiting for rebuilding.
Container::~Container(){
	delete_tree(tree, 0);

	while(tmpInstances){
		Instance *tmp = tmpInstances;
		delete tmpInstances;
		tmpInstances = tmp;
	}
}

Intersection *Container::trace(const Ray *r, Renderer *rend) const{

}

Intersection *Container::trace_limited(const Ray *r, Renderer *rend) const{

}

/// Rebuild the container's KD-tree.
/// This is not fully transaction safe -
/// it first destroys the tree and if rebuilding it fails
/// the instances remain only in the tmpObjs list.
void Container::rebuild(){
	if(rebuilt)
		return;
	
	delete_tree(t, &tmpObjs);
	t = 0;

	bbox = BoundingBox();
	cItems = 0;

	Instance *i = tmpObjs;
	while(i){
		i -> o -> rebuild();

		i -> transformedBBox = *(i -> o -> get_bounding_box());
		(i -> transformedBBox).transform(i -> t);

		bbox.enlarge(i -> transformedBBox);
		
		++cItems;

		i = i -> next;
	}

	t = buildTree(tmpInstances, &bbox, cItems);

	rebuilt = true;
}

void Container::add(Object *o, Transformation *t, bool inheritSurface);
	Instance tmp = new Instance();

	tmp -> o = o;
	tmp -> t = t;
	tmp -> inheritSurface = inheritSurface;

	tmp -> next = tmpInstances;
	tmpInstances = tmp;
	
	rebuilt = false;
}

Surface *Container::get_surface(const Point *p, const Ray *r) const{

}

/// Returns the first intersection with positive distance.
Intersection *get_limited_instersection(KDTree *t, const Ray *r, Renderer *rend, double tMax) const{
	double tx = ((t -> mid).x - (r -> origin).x) * (r -> invDirection).x;
	double ty = ((t -> mid).y - (r -> origin).y) * (r -> invDirection).y;
	double tz = ((t -> mid).z - (r -> origin).z) * (r -> invDirection).z;

	Intersection *ret = 0;
	Instance *i = t -> o;
	
	while(i){
		Ray r2 = r;
		double scale = r2.transform(i -> t -> get_inverse());

		if(i -> o -> get_bounding_box() -> has_intersection(r2)){
			Intersection *tmp = i -> o -> trace(r2, rend, true);

			if(tmp){
				tmp -> distance /= scale;
				if(!ret)
					ret = tmp;
				else if(tmp -> distance > 0. && tmp -> distance < ret -> distance)
					rend -> intersection_to_pool(ret);
					ret = tmp;
				}else
					rend -> intersection_to_pool(tmp);
			}
		}

		i = i -> next;
	}

	int currentCell = ((r -> origin).x > (t -> mid).x ? 1 : 0) |
	                  ((r -> origin).y > (t -> mid).y ? 2 : 0) |
	                  ((r -> origin).z > (t -> mid).z ? 4 : 0);

	Intersection *tmp = get_limited_intersections((t -> p)[currentCell], r, rend);
	
	if(tmp){
		if(!ret)
			return tmp;
		else if(tmp -> distance > 0. && tmp -> distance < ret -> distance)
			rend -> intersection_to_pool(ret);
			return tmp;
		}else{
			rend -> intersection_to_pool(tmp);
			return ret;
		}
	}



	enum{X = 1, Y = 2, Z = 4} axes[3];

	if(tX < tY){
		if(tY < tZ){
			axes = {X, Y, Z};
		}else if(tX < tZ){
			axes = {X, Z, Y};
		}else{
			axes = {Z, X, Y};
		}
	}else{ // tY < tX
		if(tZ < tY){
			axes = {Z, Y, X};
		else if(tX < tZ){
			axes = {Y, X, Z};
		}else{
			axes = {Y, Z, X};
		}
	}

	while(currentAxis < 3){
		Intersection *tmp = get_limited_intersections((t -> p)[currentCell], r, rend);
		++ currentAxis;

		if(!ret)
			ret = tmp;
		else if(tmp && tmp -> distance > 0. && ret -> distance > tmp -> distance)
			rend -> intersection_to_pool(ret);
			return tmp;
		}

		
	}

	return ret;
}

/// Deletes the KD-tree.
/// \arg keepInstances If this argument is not null, then puts
/// the instances from the deleted tree into *keepInstances list.
/// If it is null, then the Instance objects are deleted.
void Container::delete_tree(KDTree *t, Instance **keepInstances){
	while(t -> i){
		Instance *tmp = t -> i -> next;
		if(!keepInstances)
			delete t -> i;
		else{
			t -> i -> next = (*keepInstances);
			(*keepInstances) = t -> i;
		}

		t -> i = tmp;
	}

	delete_tree(t -> front, keepInstances);
	delete_tree(t -> back, keepInstances);
	delete t;
}

/// Checks how the result of split_by_plane will look like.
/// Tries splitting the list by three planes going through point pt
/// and perpendicular to the axes.
/// Updates the best* arguments when finds a better split.
/// (better split has no intersections of bounding boxies with the
/// plane and lower balance in absolute value).
/// \arg bestSplit Pointer to the coordinate of the best split found so far.
/// \arg bestAxis Pointer to the axis perpendicular to the best splitting plane.
/// \arg bestBalance Pointer to balance of the best split found so far
/// (balance is the number of instances in front of the splitting plane - number of instances behind).
void Container::check_split_planes(Point *pt, Instance *list, double *bestSplit, double *bestAxis, int *bestBalance){
	bool intersecting[3] = {false, false, false};
	int balance[3] = {0, 0, 0};

	while(list){
		for(Axis a = X; a <= Z; ++a){
			if((list -> transformedBBox).a[a] >= (*pt)[a]){
				++balance[a];
			}else if((list -> transformedBBox).b[a] <= (*pt)[a]){
				--balance[a];
			}else{
				intersecting[a] = true;
			}
		}

		list = list -> next;
	}

	for(Axis a = X; a <= Z; ++a)
		if(!intersecting[a] && abs(balance[a]) < abs(*bestBalance)){
			*bestSplit = (*pt)[a];
			*bestAxis = a;
			*bestBalance = balance[a];
		}
}

/// Splits the instances in the input list by a plane and returns lists of instances in front and behind the plane.
/// If instances' transformed bounding boxes intersect with the plane, then these instances stay in the input list.
void Container::split_by_plane(Instance **instances, double split, Axis axis, Instance **front, Instance **back){
	Instance *tmp;
	Instance *intersecting = 0;
	
	while(*instances){
		tmp = *instances;
		*instances = (*instances) -> next;

		if(((*instances) -> transformedBBox).a[axis] >= split){
			tmp -> next = *front;
			*front = tmp;
		}else if(((*instances) -> transformedBBox).b[axis] <= split){
			tmp -> next = *back;
			*back = tmp;
		}else{
			tmp -> next = intersexting;
			intersecting = tmp;
		}
	}

	*instances = 
}

/// Builds the KD-tree.
/// \arg instances Pointer to the list of instances from wich to build the tree.
/// The order of these instances may change if building the tree fails.
/// Instances in this list must have up to date transformed bounding boxes.
/// \arg box Bounding box of all the instances given to this function.
/// \arg count Number of objects in the list of instances.
/// \return A KD-tree.
KDTree *Container::build_tree(Instance **instances, unsigned int count){
	if(count == 0)
		return 0;
	
	KDTree *ret = new KDTree();

	double split;
	int balance = count + 1; // < worse than any balance that can happen
	Axis axis;

	if(count > 1){
		for(Instance *i = *instances; i; i = i -> next){
			check_split_planes(i -> o -> get_bounding_box() -> a, *instances, &bestSplit, &axis, &balance);
			check_split_planes(i -> o -> get_bounding_box() -> b, *instances, &bestSplit, &axis, &balance);
		}
	}
	
	if(abs(balance) >= count){
		// there's no point in splitting the tree if the one branch would be empty
		ret -> front = ret -> back = 0;
		ret -> i = *instances;

		return ret;
	}

	Instance *front = 0;
	Instance *back = 0;
	
	split_by_plane(instances, split, axis, &front, &back);

	assert(!*instances);

	try{
		ret -> front = build_tree(&front, (count + balance) / 2);
	}catch(...){
		delete ret;

		slow_append(&front, back);
		*instances = front;

		throw;
	}

	try{
		ret -> back = build_tree(&back, (count - balance) / 2);
	}catch{
		delete ret;

		delete_tree(ret -> front, &back);
		*instances = back;

		throw;
	}

	ret -> instances = 0;
	ret -> axis = axis;
	ret -> coord = split;
}

/// Merge sort.
void Container:IL_sort(Intersection2 **list){
	// split it
	Intersection *l1 = 0;
	Intersection *l2 = 0;

	Intersection *tmp1, *tmp2;

	if(!(*list))
		return;
	
	tmp1 = (*list);
	tmp2 = tmp2 -> next;

	while(tmp2){
		tmp2 -> next = l1;
		l1 = tmp1;

		tmp1 = tmp2 -> next;
		tmp2 -> next = l2;
		l2 = tmp2;

		tmp2 = tmp1 -> next;
	}

	if(tmp1){
		tmp1 -> next = l1;
		l1 = tmp1;
	}

	IL_sort(&l1);
	IL_sort(&l2);

	IL_merge(&l1, l2, 0);

	(*list) = l1;
}

/// Merge l2 into *out.
/// @arg last Pointer to pointer to the next pointer of the last node is returned here.
/// even after the merge.
void Container::IL_merge(Intersection **out, Intersection2 *l2, Intersection ***last){
	if(l2 -> distance < (*out) -> distance)
		swap(*out, l2);

	Intersection *l1 = *out;
	Intersection **tmpLast;
	
	while(l1){
		Intersection *tmp = l1 -> next;
		while(l2 && l2 -> distance <= l1 -> distance){
			l1 -> next = l2;
			l2 = l2 -> next;
			l1 = l1 -> next;
			l1 -> next = tmp;
		}

		tmpLast = &(l1 -> next);
		l1 = l1 -> next;
	}

	if(last)
		(*last) = tmpLast;
}

void IL_delete(Intersection2 *list){
	while(list){
		Intersection2 *tmp = list -> next;
		delete list;
		list = tmp;
	}
}
