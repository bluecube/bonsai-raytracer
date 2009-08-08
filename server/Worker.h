#ifndef WORKER_H
#define WORKER_H

/// A class for a single working thread.
class Worker{
	Worker() : pool(0), poolEnd(&pool) {}

	Intersection *intersection_from_pool();
	void intersection_to_pool(Intersection *i);
	void intersections_to_pool(Intersection *i, Intersection *last);

	void render(Scene *s, Pixmap *p, unsigned int rowFrom, unsigned int rowTo);

	void clear_pool();
private:
	Intersection *pool, **poolEnd;
};

#endif
