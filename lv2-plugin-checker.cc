//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "lv2-plugin-checker.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

static double sample_rate = 44100;
static size_t buffer_size = 16;
static bool test_plugin(LilvWorld &world, const char *uri);

int main(int argc, char *argv[])
{
    for (int c; (c = getopt(argc, argv, "b:s:")) != -1;) {
        switch (c) {
        case 's':
            sample_rate = strtod(optarg, nullptr);
            break;
        case 'b':
            buffer_size = strtoul(optarg, nullptr, 0);
            break;
        default:
            return 1;
        }
    }

    char **uris = argv + optind;
    if (!*uris) {
        fprintf(stderr, "No URI has been given!\n");
        return 1;
    }

    LilvWorld_u world(lilv_world_new());
    lilv_world_load_all(world.get());

    fprintf(stderr, "The sample rate is %g\n", sample_rate);

    size_t success = 0;
    size_t failure = 0;

    for (char **p_uris = uris; *p_uris; ++p_uris) {
        const char *uri = *p_uris;
        if (test_plugin(*world, uri))
            ++success;
        else
            ++failure;
    }

    return (failure == 0) ? 0 : 1;
}

bool test_plugin(LilvWorld &world, const char *uri)
{
    fprintf(stderr, "* Plugin: %s\n", uri);

    const LilvPlugins *plugins = lilv_world_get_all_plugins(&world);
    LilvNode_u uri_node(lilv_new_uri(&world, uri));

    const LilvPlugin *plugin = lilv_plugins_get_by_uri(plugins, uri_node.get());
    if (!plugin) {
        fprintf(stderr, "Cannot find the plugin\n");
        return false;
    }

    LilvInstance_u instance(lilv_plugin_instantiate(plugin, sample_rate, nullptr));
    if (!plugin) {
        fprintf(stderr, "Could not instantiate the plugin\n");
        return false;
    }

    uint32_t num_ports = lilv_plugin_get_num_ports(plugin);
    std::unique_ptr<float[]> buffers(new float[num_ports * buffer_size]());

    for (uint32_t i = 0; i < num_ports; ++i)
        lilv_instance_connect_port(instance.get(), i, &buffers[i * buffer_size]);

    //
    lilv_instance_activate(instance.get());
    for (unsigned i = 0; i < 10; ++i)
        lilv_instance_run(instance.get(), buffer_size);
    lilv_instance_deactivate(instance.get());

    for (unsigned i = 0; i < 10; ++i) {
        lilv_instance_activate(instance.get());
        lilv_instance_deactivate(instance.get());
    }

    lilv_instance_activate(instance.get());
    for (unsigned i = 0; i < 10; ++i)
        lilv_instance_run(instance.get(), buffer_size);
    lilv_instance_deactivate(instance.get());

    return true;
}
