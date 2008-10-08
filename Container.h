#ifndef CONTAINER_H
#define CONTAINER_H

#include "Object.h"
#include "Instance.h"

/// Container object.
/// Uses KD-tree to store the subnodes.
/// Works in a few different CSG modes, these are set with set_mode().
/// \todo Should lights be also stored in containers?
/// (a car with headlights on should be transformed all at once ...)
class Container : public Object{
public:
	/// Container mode enum.
	enum Mode{
		OR, /// < Container represents the union of objects.
		AND, /// < Container represents the intersection of objects.
		NOT_OR, /// < Container represents the negation of union of objects.
		DIFF, /// < Container represents the difference of objects.
		      /// < In this mode only two objects can be in the container.
		      /// < For use in this mode, use add_positive() and add_negative()
		      /// < instead of add().
	};

	Container(Mode m = OR);
	~Container();

	Intersection *trace(const Ray *r, Renderer *rend, bool limit) const;

	void rebuild();

	void add(Object *o, Transformation *t, bool inheritSurface);

	void add_positive(Object *o, Transformation *t, bool inheritSurface);
	void add_negative(Object *o, Transformation *t, bool inheritSurface);
private:
	/// A instance of a object in the container.
	struct Instance{
		Object *o;
		Transformation *t;

		bool inheritSurface;

		/// Precomputed transformed bounding box of the object.
		BoundingBox transformedBBox;

		Instance *next;
	};

	enum Axis{
		NONE = -1.
		X,
		Y,
		Z,
	};

	/// The KD-tree node.
	struct KDTree{
		/// \todo build_tree and delete_tree as KDTree methods?
		/// How would keepInstances be done?

		KDTree *front, *back;

		Axis axis;
		double coord;

		Instance *i;

	};

	Surface *get_surface(const Point *p, const Ray *r) const;

	Intersection *get_limited_instersection(KDTree *t, const Ray *r, Renderer *rend, double tMin, double tMax) const;
	Intersection *get_all_instersections(KDTree *t, const Ray *r, Renderer *rend, double tMin, double tMax) const;

	void check_split_planes(Point *pt, Instance *list, double *bestSplit, double *bestAxis, int *bestBalance);

	void delete_tree(KDTree *t, Instance **keepInstances);
	KDTree *build_tree(Instance *i, BoundingBox *box, unsigned int count);

	KDTree *tree;
	Instance *tmpInstances;

	Mode mode;
	bool rebuilt;

	unsigned int cItems;
};

#endif
