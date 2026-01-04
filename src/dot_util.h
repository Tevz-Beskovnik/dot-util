#pragma once
/// TODO: Add styling types for nodes and connections so styles don't need to be redefiend for every created node and connecton + for graphs

#include <string>
#include <unordered_map>
#include <vector>

namespace dot 
{
    struct generic_dot_element_t
    {
        virtual std::string to_dot() = 0;
    };

    enum shape_t
    {
        box,
        ellipse,
        oval,
        circle,
        triangle,
        diamond,
        record
    };

    enum graph_type_t
    {
        graph,
        digraph
    };

    struct node_style_t : generic_dot_element_t
    {
        shape_t shape = box;
        std::string color = "#000000";
        std::string fill = "#FFFFFF";

        node_style_t() = default;

        node_style_t(shape_t shape, std::string color, std::string fill);

        std::string to_dot() final;
    };

    struct generic_node_t : generic_dot_element_t
    {
        std::string label;
       
        protected: 

        node_style_t default_style = node_style_t(shape_t::box, "#000000", "#FFFFFF");
    };

    struct node_t final : generic_node_t 
    {
        std::string name;
        node_style_t *style = &default_style;
        
        node_t(std::string name, node_style_t *style = nullptr);
        
        std::string to_dot() final;

        static node_t *create_managed(std::string name, node_style_t *style = nullptr);
    };

    struct multy_node_t final : generic_node_t
    {
        std::vector<std::string> vals;
        node_style_t *style = &default_style;

        multy_node_t(std::string label, std::vector<std::string> vals, node_style_t *style = nullptr);

        std::string to_dot() final;
    
        static multy_node_t *create_managed(std::string label, std::vector<std::string> vals, node_style_t *style = nullptr);
    };

    struct connection_t final : generic_dot_element_t
    {
        std::string from;
        std::string to;
        std::string label;
        
        connection_t(std::string &from, std::string &to, std::string &label);
        
        connection_t(std::string &from, std::string &to);

        std::string to_dot() final;
    };

    struct graph_t : generic_dot_element_t
    {
        graph_type_t type;
        std::string name;
        std::string bg_color = "white";

        graph_t(graph_type_t type, std::string graph_name);

        void add_node(generic_node_t *node); // this is bad, but for now thats how we roll

        void add_connection(std::string from, std::string to, std::string label);
        
        void add_connection(std::string from, std::string to);

        std::string to_dot() override;

        void to_file(const char * const filename);

        void display();

        virtual ~graph_t();

        private:

        std::unordered_map<std::string, generic_node_t *> node_map;
        std::vector<generic_node_t *> nodes;
        std::vector<connection_t> connections;
    };
}

