//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <lilv/lilv.h>
#include <memory>

struct Lilv_Deleter {
    void operator()(LilvWorld *x)
        { lilv_world_free(x); }
    void operator()(LilvNode *x)
        { lilv_node_free(x); }
    void operator()(LilvInstance *x)
        { lilv_instance_free(x); }
};

typedef std::unique_ptr<LilvWorld, Lilv_Deleter> LilvWorld_u;
typedef std::unique_ptr<LilvNode, Lilv_Deleter> LilvNode_u;
typedef std::unique_ptr<LilvInstance, Lilv_Deleter> LilvInstance_u;
