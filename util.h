#ifndef UTIL_H
#define UTIL_H

#include <exception>
#include <QPointF>

struct DataLoadingException : public std::exception {
   const char * what () const throw () {
      return "Custom";
   }
};


double euclid_distance(QPointF * point1, QPointF * point2);


#endif // UTIL_H
