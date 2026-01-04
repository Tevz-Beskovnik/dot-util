#include "nob.h"
#include "src/dot_util.h"

int main (int argc, char *argv[]) {
    REBUILD_SELF_AND_WATCH(argc, argv, "./src/dot_util.cpp");

    dot::graph_t graph(dot::graph_type_t::digraph, "example");

    dot::node_style_t style;
    style.color = "black";
    style.fill = "white";
    style.shape = dot::circle;

    dot::node_style_t style_o;
    style.color = "black";
    style.fill = "#FF0000";
    style.shape = dot::circle;

    dot::node_style_t style_i;
    style.color = "black";
    style.fill = "#0000FF";
    style.shape = dot::circle;

    dot::node_style_t style_t;
    style.color = "black";
    style.fill = "#FFFF00";
    style.shape = dot::circle;

    graph.add_node(dot::node_t::create_managed("4", &style));
    graph.add_node(dot::node_t::create_managed("2", &style_o));
    graph.add_node(dot::node_t::create_managed("10", &style));
    graph.add_node(dot::node_t::create_managed("6", &style_t));
    graph.add_node(dot::node_t::create_managed("8", &style_t));
    graph.add_node(dot::node_t::create_managed("22", &style));
    graph.add_node(dot::node_t::create_managed("1", &style_i));

    graph.add_connection("1", "4");
    graph.add_connection("1", "10");
    graph.add_connection("1", "2");
    graph.add_connection("1", "22");
    graph.add_connection("4", "6");
    graph.add_connection("8", "1");
    graph.add_connection("22", "8");
    graph.add_connection("10", "2");
    graph.add_connection("2", "8");

    graph.display();
    
    return 0;
}
