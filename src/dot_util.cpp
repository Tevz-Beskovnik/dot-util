#include "dot_util.h"
#include "../nob.h"
#include <cassert>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace dot
{
    std::string to_shape_string(shape_t shape)
    {
        switch (shape) {
            case box: 
                return "box";
            case ellipse:
                return "ellipse";
            case oval:
                return "oval";
            case circle:
                return "circle";
            case triangle:
                return "triangle";
            case diamond:
                return "diamond";
            case record:
                return "record";
        }

        throw std::runtime_error("Invalid node shape");
    }

    node_style_t::node_style_t(shape_t shape, std::string color, std::string fill)
        :shape(shape), color(color), fill(fill)
    {
        ;
    }

    std::string node_style_t::to_dot()
    {
        std::stringstream ss;

        ss << "color = \"" << color << "\", ";
        ss << "fillcolor = \"" << fill << "\", ";
        ss << "shape = " << to_shape_string(shape);

        return ss.str();
    }


    node_t::node_t(std::string name, node_style_t *style)
        :name(name)
    {
        label = name;

        if(style != nullptr)
        {
            this->style = style;
        }
    }
        
    node_t *node_t::create_managed(std::string name, node_style_t *style)
    {
        return new node_t(name, style);
    }
        
    std::string node_t::to_dot()
    {
        std::stringstream ss;
        std::string node_style = style->to_dot();
    
        ss << name << " [ " << node_style << "]";

        return ss.str();
    }

    multy_node_t::multy_node_t(std::string label, std::vector<std::string> vals, node_style_t *style)
        :vals(vals)
    {
        this->label = label;
        if(style != nullptr)
            this->style = style;
    }

    std::string multy_node_t::to_dot()
    {
        std::stringstream ss;
        ss << label << " [ label=\"";

        for(size_t i = 0; i < vals.size(); i++)
        {
            ss << "<f" << i << "> |" << vals[i] << "|";
        }

        ss << "<f" << vals.size() << ">\", ";
        ss << style->to_dot() << ", width=" << static_cast<float>(vals.size())/2 << "]";

        return ss.str();
    }
    
    multy_node_t *multy_node_t::create_managed(std::string label, std::vector<std::string> vals, node_style_t *style)
    {
        return new multy_node_t(label, vals, style);
    }

    connection_t::connection_t(std::string &from, std::string &to, std::string &label)
        :from(from), to(to), label(label)
    {
        ;
    }
    
    connection_t::connection_t(std::string &from, std::string &to)
        :from(from), to(to), label("")
    {
        ;
    }

    std::string connection_t::to_dot()
    {
        std::stringstream ss;

        ss << from << " -> " << to << " [arrowhead = normal";

        if(label != "") ss << ", label = \"" << label << "\"";

        ss << "]";
        
        return ss.str();
    }

    graph_t::graph_t(graph_type_t type, std::string graph_name)
        :type(type), name(graph_name), bg_color("#FFFFFF")
    {
        ;
    }

    void graph_t::add_node(generic_node_t *node)
    {
        if(node == nullptr) throw std::invalid_argument("Node is null");

        if(node_t *node_p = dynamic_cast<node_t *>(node); node_p != nullptr)
        {
            if(node_map.contains(node->label)) 
            {
                delete node_p;
                return;
            }

            nodes.push_back(node);
            node_map[node->label] = node;
        }
        else if(multy_node_t *multy_p = dynamic_cast<multy_node_t *>(node); multy_p != nullptr)
        {
            if(node_map.contains(node->label))
            {
                delete multy_p;
                return;
            }

            nodes.push_back(node);
            node_map[node->label] = node;

            for(size_t i = 0; i <= multy_p->vals.size(); i++)
            {
                std::stringstream full_name;
                full_name << "\"" << node->label << "\"" << ":f" << i;
                node_map[full_name.str()] = node;
            }
        }
    }

    void graph_t::add_connection(std::string from, std::string to, std::string label)
    {
        assert(node_map.contains(from) && node_map.contains(to));

        connections.push_back(connection_t(from, to, label));
    }
        
    void graph_t::add_connection(std::string from, std::string to)
    {
        assert(node_map.contains(from) && node_map.contains(to));

        connections.push_back(connection_t(from, to));
    }

    std::string graph_t::to_dot()
    {
        std::stringstream ss;

        if(type == graph_type_t::graph) ss << "graph";
        else if(type == graph_type_t::digraph) ss << "digraph";

        ss << " " << name << " {\n";

        ss << "graph [\n"
        "  charset = \"UTF-8\",\n"
        "  label = \"" << name << "\",\n"
        "  labelloc = \"t\",\n"
        "  labeljust = \"c\",\n"
        "  bgcolor = \"" << bg_color << "\",\n"
        "  fontcolor = \"#343434\",\n"
        "  fontsize = 18,\n"
        "  style = \"filled\",\n"
        "  rankdir = TB,\n"
        "  margin = 0.2,\n"
        "  splines = spline,\n"
        "  ranksep = 1.0,\n"
        "  nodesep = 0.9\n"
        "];\n"
        "ranksep=1\n";

        ss << "node ["
        "  colorscheme = \"rdylgn11\"\n"
        "  style = \"solid,filled\",\n"
        "  fontsize = 16,\n"
        "  fontcolor = \"#343434\",\n"
        "  fontname = \"Migu 1M\",\n"
        "  color = black,\n"
        "  fillcolor = white,\n"
        "  fixedsize = true,\n"
        "  height = 0.6,\n"
        "  width = 1.2\n"
        "];\n";

        ss<< "edge [\n"
            "  style = solid,\n"
            "  fontsize = 14,\n"
            "  fontcolor = \"#343434\",\n"
            "  fontname = \"Migu 1M\",\n"
            "  color = black,\n"
            "  labelfloat = true,\n"
            "  labeldistance = 2.5,\n"
            "  labelangle = 70\n"
            "];\n";

        for(const auto &node : nodes)
        {
            ss << node->to_dot() << "\n";
        }

        ss << "\n";

        for(auto &connection : connections)
        {
            ss << connection.to_dot() << "\n";
        }

        ss << "\n}\n";

        return ss.str();
    }

    void graph_t::to_file(const char * const filename)
    {
        std::ofstream file(filename);

        file << to_dot();

        file.close();
    }

    void graph_t::display()
    {
        std::string gv_name = name + ".gv";
        std::string img_name = name + ".png";

        to_file(gv_name.c_str());

        COMMAND("dot", "-Tpng", gv_name.c_str(), "-o", img_name.c_str());
        COMMAND_ASYNC("loupe", img_name.c_str());
    }

    graph_t::~graph_t()
    {
        for(const auto &node : nodes)
        {
            if(node_t *node_p = dynamic_cast<node_t *>(node); node_p != nullptr)
                delete node_p;
            else
                delete dynamic_cast<multy_node_t *>(node);
        }
    }
}
