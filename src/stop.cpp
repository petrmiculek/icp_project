/* stop.cpp
 * Project: CPP
 * Description: Stop class
 * Author: Petr Mičulek, FIT <xmicul08@stud.fit.vutbr.cz>
 */

#include "stop.h"
#include "util.h"

Stop::Stop(int id, double street_percentage, QString name) :
    id(id),
    street_percentage(street_percentage),
    name(ToCamelCase(name))
{

}
