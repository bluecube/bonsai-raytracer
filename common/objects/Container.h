#ifndef CONTAINER_H
#define CONTAINER_H

#include "common/objects/Object.h"
#include "common/LList.h"

/// Container object.
/// Does the logical OR on objects it contains.
/// Uses KD-tree to store them.
/// \todo Should lights be also stored in containers?
/// (a car with headlights on should be transformed all at once ...)
class Container : public Object{
public:
	Container();
	~Container();

	LList<Intersection> trace(const Ray *r, Renderer *rend) const;
	bool trace_limited(const Ray *r, Intersection *ret, Renderer *rend, Intersection * out) const;

	void rebuild();

	void add(Object *o, Transformation *t, bool inheritSurface);
protected:
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
		NONE = -1,
		X = 0,
		Y = 1,
		Z = 2,
	};

	/// The KD-tree node.
	struct KDTree{
		KDTree *front, *back;

		Axis axis;
		double coord;

		LList<Instance> instances;
	};

	Surface *get_surface(const Point3D *p, const Ray *r) const;

	double get_limited_instersection(KDTree *t, const Ray *r, Renderer *rend, double tMin, double tMax) const;
	Intersection *get_all_instersections(KDTree *t, const Ray *r, Renderer *rend, double tMin, double tMax) const;

	void check_split_planes(Point3D *pt, Instance *list, double *bestSplit, double *bestAxis, int *bestBalance);

	void delete_tree(KDTree *t, Instance **keepInstances);
	KDTree *build_tree(Instance *i, BoundingBox *box, unsigned int count);

	KDTree *tree;
	Instance *tmpInstances;

	bool rebuilt;

	unsigned int cItems;
};

#endif
