#include "math.h"
#include "ruby.h"
#include "igraph.h"

//#define DEBUG

//Classes
extern VALUE cIGraph;
extern VALUE cIGraphError;
extern VALUE cIGraphMatrix;
extern igraph_attribute_table_t cIGraph_attribute_table;

//Error and warning handling functions
void cIGraph_error_handler(const char *reason, const char *file,
			   int line, int igraph_errno);
void cIGraph_warning_handler(const char *reason, const char *file,
                             int line, int igraph_errno);

//IGraph specific utility functions
igraph_integer_t cIGraph_get_vertex_id(VALUE graph, VALUE v);
VALUE cIGraph_get_vertex_object(VALUE graph, igraph_integer_t n);
int cIGraph_vertex_arr_to_id_vec(VALUE graph, VALUE va, igraph_vector_t *nv);
VALUE cIGraph_include(VALUE self, VALUE v);

//IGraph allocation, destruction and intialization
void Init_igraph(void);
void cIGraph_free(void *p);
void cIGraph_mark(void *p);
VALUE cIGraph_alloc(VALUE klass);
VALUE cIGraph_initialize(int argc, VALUE *argv, VALUE self);
VALUE cIGraph_init_copy(VALUE copy, VALUE orig);

//Graph generators
VALUE cIGraph_adjacency(VALUE self, VALUE matrix, VALUE mode);
VALUE cIGraph_star     (VALUE self, VALUE n,      VALUE mode, VALUE center);
VALUE cIGraph_lattice  (VALUE self, VALUE dim, VALUE directed, VALUE mutual, VALUE circular);
VALUE cIGraph_ring(VALUE self, VALUE n, VALUE directed, VALUE mutual, VALUE circular);
VALUE cIGraph_tree(VALUE self, VALUE n, VALUE children, VALUE type);
VALUE cIGraph_full(VALUE self, VALUE n, VALUE directed, VALUE loops);
VALUE cIGraph_atlas(VALUE self, VALUE n);
VALUE cIGraph_extended_chordal_ring(VALUE self, VALUE n, VALUE matrix);
VALUE cIGraph_connect_neighborhood(VALUE self, VALUE order, VALUE mode);

//Random graph generators
VALUE cIGraph_grg_game     (VALUE self, VALUE nodes, 
			    VALUE radius, VALUE torus);
VALUE cIGraph_barabasi_game(VALUE self, VALUE nodes, 
			    VALUE m, VALUE outpref, VALUE directed);
VALUE cIGraph_nonlinear_barabasi_game(VALUE self, VALUE nodes, VALUE power, 
				      VALUE m, VALUE outpref, VALUE zeroappeal, VALUE directed);
VALUE cIGraph_erdos_renyi_game       (VALUE self, VALUE type, VALUE nodes, VALUE mp, VALUE directed, VALUE loops);
VALUE cIGraph_watts_strogatz_game(VALUE self, VALUE dim, VALUE size, VALUE nei, VALUE p);
VALUE cIGraph_degree_sequence_game(VALUE self, VALUE out_deg, VALUE in_deg);
VALUE cIGraph_growing_random_game(VALUE self, VALUE n, VALUE m, VALUE directed, VALUE citation);
VALUE cIGraph_callaway_traits_game(VALUE self, VALUE nodes, VALUE types, VALUE e_per_step, VALUE type_dist, VALUE pref_matrix, VALUE directed);
VALUE cIGraph_establishment_game(VALUE self, VALUE nodes, VALUE types, VALUE k, VALUE type_dist, VALUE pref_matrix, VALUE directed);
VALUE cIGraph_preference_game(VALUE self, VALUE nodes, VALUE types, VALUE type_dist, VALUE pref_matrix, VALUE directed, VALUE loops);
VALUE cIGraph_asymmetric_preference_game(VALUE self, VALUE nodes, VALUE types, VALUE type_dist_matrix, VALUE pref_matrix, VALUE loops);
VALUE cIGraph_recent_degree_game(VALUE self, VALUE n, VALUE power, VALUE window, VALUE m, VALUE outpref, VALUE zero_appeal, VALUE directed);
VALUE cIGraph_barabasi_aging_game(VALUE self, VALUE nodes, VALUE m, VALUE outpref, VALUE pa_exp, VALUE aging_exp, VALUE aging_bin, VALUE zero_deg_appeal, VALUE zero_age_appeal, VALUE deg_coef, VALUE age_coef, VALUE directed);
VALUE cIGraph_recent_degree_aging_game(VALUE self, VALUE nodes, VALUE m, VALUE outpref, VALUE pa_exp, VALUE aging_exp, VALUE aging_bin, VALUE time_window, VALUE zero_appeal, VALUE directed);
VALUE cIGraph_cited_type_game(VALUE self, VALUE nodes, VALUE types, VALUE pref, VALUE e_per_s, VALUE directed);
VALUE cIGraph_citing_cited_type_game(VALUE self, VALUE nodes, VALUE types, VALUE pref, VALUE e_per_s, VALUE directed);

VALUE cIGraph_rewire_edges(VALUE self, VALUE prop);
VALUE cIGraph_rewire(VALUE self, VALUE n);

//Attribute accessors
int replace_i(VALUE key, VALUE val, VALUE hash);
VALUE cIGraph_get_edge_attr(VALUE self, VALUE from, VALUE to);
VALUE cIGraph_set_edge_attr(VALUE self, VALUE from, VALUE to, VALUE attr);
VALUE cIGraph_graph_attributes(VALUE self);
igraph_i_attribute_record_t cIGraph_create_record(VALUE v);

//Iterators
VALUE cIGraph_each_vertex  (VALUE self);
VALUE cIGraph_each_edge    (VALUE self, VALUE order);
VALUE cIGraph_each_edge_eid(VALUE self, VALUE order);

//Selectors
VALUE cIGraph_all_v   (VALUE self);
VALUE cIGraph_adj_v   (VALUE self, VALUE v, VALUE mode);
VALUE cIGraph_nonadj_v(VALUE self, VALUE v, VALUE mode);

VALUE cIGraph_all_e   (VALUE self,          VALUE mode);
VALUE cIGraph_adj_e   (VALUE self, VALUE v, VALUE mode);

//Basic query operations
VALUE cIGraph_vcount     (VALUE self);
VALUE cIGraph_ecount     (VALUE self);
VALUE cIGraph_edge       (VALUE self, VALUE eid);
VALUE cIGraph_get_eid    (VALUE self, VALUE from, VALUE to, VALUE directed);
VALUE cIGraph_neighbors  (VALUE self, VALUE v, VALUE mode);
VALUE cIGraph_adjacent   (VALUE self, VALUE v, VALUE mode);
VALUE cIGraph_is_directed(VALUE self);
VALUE cIGraph_degree     (VALUE self, VALUE v, VALUE mode, VALUE loops);

//Adding and eleting vertices and edges
VALUE cIGraph_add_edges      (int argc, VALUE *argv, VALUE self);
VALUE cIGraph_add_edge       (int argc, VALUE *argv, VALUE self);
VALUE cIGraph_add_vertices   (VALUE self, VALUE vs);
VALUE cIGraph_delete_edge    (VALUE self, VALUE from, VALUE to);
VALUE cIGraph_delete_edges   (VALUE self, VALUE edges);
VALUE cIGraph_delete_vertices(VALUE self, VALUE vs);
VALUE cIGraph_delete_vertex  (VALUE self, VALUE v);
VALUE cIGraph_add_vertex     (VALUE self, VALUE v);

//Basic properties
VALUE cIGraph_are_connected(VALUE self, VALUE from, VALUE to);

//Shortest Path Related Functions
VALUE cIGraph_shortest_paths        (VALUE self, VALUE from, VALUE mode);
VALUE cIGraph_get_shortest_paths    (VALUE self, VALUE from, VALUE to, VALUE mode);
VALUE cIGraph_get_all_shortest_paths(VALUE self, VALUE from, VALUE to, VALUE mode); 
VALUE cIGraph_average_path_length   (VALUE self, VALUE directed, VALUE unconn);
VALUE cIGraph_diameter              (VALUE self, VALUE directed, VALUE unconn);
VALUE cIGraph_girth                 (VALUE self);

VALUE cIGraph_dijkstra_shortest_paths(VALUE self, VALUE from, VALUE weights, VALUE mode);
VALUE cIGraph_get_dijkstra_shortest_paths(VALUE self, VALUE from, VALUE to, VALUE weights, VALUE mode);
int igraph_dijkstra_shortest_paths(const igraph_t *graph, 
				   igraph_matrix_t *res, 
				   const igraph_vs_t from, 
				   const igraph_vector_t *wghts,
				   igraph_neimode_t mode);

//Vertex neighbourhood functions
VALUE cIGraph_neighborhood_size  (VALUE self, VALUE from, VALUE order, VALUE mode);
VALUE cIGraph_neighborhood       (VALUE self, VALUE from, VALUE order, VALUE mode);
VALUE cIGraph_neighborhood_graphs(VALUE self, VALUE from, VALUE order, VALUE mode);

//Component functions
VALUE cIGraph_subcomponent(VALUE self, VALUE v, VALUE mode);
VALUE cIGraph_subgraph    (VALUE self, VALUE vs);
VALUE cIGraph_clusters    (VALUE self, VALUE mode);
VALUE cIGraph_decompose   (int argc, VALUE *argv, VALUE self);

//Centrality measures
VALUE cIGraph_closeness       (VALUE self, VALUE vs, VALUE mode);
VALUE cIGraph_betweenness     (VALUE self, VALUE vs, VALUE directed);
VALUE cIGraph_edge_betweenness(VALUE self, VALUE directed);
VALUE cIGraph_pagerank        (VALUE self, VALUE vs, VALUE directed, VALUE niter, VALUE eps, VALUE damping);
VALUE cIGraph_constraint      (int argc, VALUE *argv, VALUE self);
VALUE cIGraph_maxdegree       (VALUE self, VALUE vs, VALUE mode, VALUE loops);

//Spanning trees
VALUE cIGraph_minimum_spanning_tree_prim      (VALUE self, VALUE weights);
VALUE cIGraph_minimum_spanning_tree_unweighted(VALUE self);

//Transitivity
VALUE cIGraph_transitivity         (VALUE self);
VALUE cIGraph_transitivity_local   (VALUE self, VALUE vs);
VALUE cIGraph_transitivity_avglocal(VALUE self);

//Directedness conversion
VALUE cIGraph_to_directed  (VALUE self, VALUE mode);
VALUE cIGraph_to_undirected(VALUE self, VALUE mode);

//Spectral properties
VALUE cIGraph_laplacian(VALUE self, VALUE mode);

//K-Cores
VALUE cIGraph_coreness(VALUE self, VALUE mode);

//Topological sorting
VALUE cIGraph_topological_sorting(VALUE self, VALUE mode);

//Other operations
VALUE cIGraph_density      (VALUE self, VALUE loops);
VALUE cIGraph_simplify     (VALUE self, VALUE mult, VALUE loops);
VALUE cIGraph_reciprocity  (VALUE self, VALUE loops);
VALUE cIGraph_bibcoupling  (VALUE self, VALUE vs);
VALUE cIGraph_cocitation   (VALUE self, VALUE vs);
VALUE cIGraph_get_adjacency(VALUE self, VALUE mode);

//Cliques
VALUE cIGraph_cliques(VALUE self, VALUE min, VALUE max);
VALUE cIGraph_largest_cliques(VALUE self);
VALUE cIGraph_maximal_cliques(VALUE self);
VALUE cIGraph_clique_number(VALUE self);

//Independent vertex sets
VALUE cIGraph_independent_vertex_sets(VALUE self, VALUE min, VALUE max);
VALUE cIGraph_largest_independent_vertex_sets(VALUE self);
VALUE cIGraph_maximal_independent_vertex_sets(VALUE self);
VALUE cIGraph_independence_number(VALUE self);

//Graph isomorphism
VALUE cIGraph_isomorphic       (VALUE self, VALUE g);
VALUE cIGraph_isomorphic_vf2   (VALUE self, VALUE g);
VALUE cIGraph_isoclass         (VALUE self);
VALUE cIGraph_isoclass_subgraph(VALUE self, VALUE vs);
VALUE cIGraph_isoclass_create  (VALUE self, VALUE vn, VALUE iso, VALUE dir);

//Motifs
VALUE cIGraph_motifs_randesu         (VALUE self, VALUE size, VALUE cuts);
VALUE cIGraph_motifs_randesu_no      (VALUE self, VALUE size, VALUE cuts);
VALUE cIGraph_motifs_randesu_estimate(VALUE self, VALUE size, VALUE cuts, 
				      VALUE samplen, VALUE samplev);

//File handling
VALUE cIGraph_read_graph_edgelist (VALUE self, VALUE file, VALUE mode);
VALUE cIGraph_write_graph_edgelist(VALUE self, VALUE file);
VALUE cIGraph_read_graph_ncol     (VALUE self, VALUE file, VALUE predefnames, VALUE names, VALUE weights, VALUE directed);
VALUE cIGraph_write_graph_ncol    (VALUE self, VALUE file, VALUE names, VALUE weights);
VALUE cIGraph_read_graph_lgl      (VALUE self, VALUE file, VALUE names, VALUE weights);
VALUE cIGraph_write_graph_lgl     (VALUE self, VALUE file, VALUE names, VALUE weights, VALUE isolates);
VALUE cIGraph_read_graph_dimacs   (VALUE self, VALUE file, VALUE directed);
VALUE cIGraph_write_graph_dimacs  (VALUE self, VALUE file, VALUE source, VALUE target, VALUE capacity);
VALUE cIGraph_read_graph_graphdb  (VALUE self, VALUE file, VALUE directed);
VALUE cIGraph_read_graph_graphml  (VALUE self, VALUE file, VALUE index);
VALUE cIGraph_write_graph_graphml (VALUE self, VALUE file);
VALUE cIGraph_read_graph_gml      (VALUE self, VALUE file);
VALUE cIGraph_write_graph_gml     (VALUE self, VALUE file);
VALUE cIGraph_read_graph_pajek    (VALUE self, VALUE file);
VALUE cIGraph_write_graph_pajek   (VALUE self, VALUE file);

//Layouts
VALUE cIGraph_layout_random              (VALUE self);
VALUE cIGraph_layout_circle              (VALUE self);
VALUE cIGraph_layout_fruchterman_reingold(VALUE self,
					  VALUE niter,
					  VALUE maxdelta,
					  VALUE area,
					  VALUE coolexp,
					  VALUE repulserad,
					  VALUE use_seed);
VALUE cIGraph_layout_kamada_kawai        (VALUE self,
				          VALUE niter,
				          VALUE sigma,
				          VALUE initemp,
				          VALUE coolexp,
				          VALUE kkconst);
VALUE cIGraph_layout_reingold_tilford    (VALUE self,
				          VALUE root);
VALUE cIGraph_layout_reingold_tilford_circular(VALUE self,
					       VALUE root);
VALUE cIGraph_layout_grid_fruchterman_reingold(VALUE self,
					       VALUE niter,
					       VALUE maxdelta,
					       VALUE area,
					       VALUE coolexp,
					       VALUE repulserad,
					       VALUE cellsize,
					       VALUE use_seed);
VALUE cIGraph_layout_lgl(VALUE self,
			 VALUE maxit,
			 VALUE maxdelta,
			 VALUE area,
			 VALUE coolexp,
			 VALUE repulserad,
			 VALUE cellsize,
			 VALUE proot);

VALUE cIGraph_layout_random_3d(VALUE self);
VALUE cIGraph_layout_sphere   (VALUE self);
VALUE cIGraph_layout_fruchterman_reingold_3d(VALUE self,
					     VALUE niter,
					     VALUE maxdelta,
					     VALUE volume,
					     VALUE coolexp,
					     VALUE repulserad);
VALUE cIGraph_layout_kamada_kawai_3d        (VALUE self,
					     VALUE niter,
					     VALUE sigma,
					     VALUE initemp,
					     VALUE coolexp,
					     VALUE kkconst);

VALUE cIGraph_layout_merge_dla(VALUE self, VALUE graphs, VALUE layouts);

//Min cuts
VALUE cIGraph_maxflow_value  (VALUE self, VALUE source, VALUE target, VALUE capacity);
VALUE cIGraph_st_mincut_value(VALUE self, VALUE source, VALUE target, VALUE capacity);
VALUE cIGraph_mincut_value   (VALUE self, VALUE capacity);
VALUE cIGraph_mincut         (VALUE self, VALUE capacity);

//Connectivity
VALUE cIGraph_st_edge_connectivity  (VALUE self, VALUE source, VALUE target);
VALUE cIGraph_edge_connectivity     (VALUE self);
VALUE cIGraph_st_vertex_connectivity(VALUE self, VALUE source, VALUE target, VALUE neighbours);
VALUE cIGraph_vertex_connectivity   (VALUE self);
VALUE cIGraph_edge_disjoint_paths   (VALUE self, VALUE source, VALUE target);
VALUE cIGraph_vertex_disjoint_paths (VALUE self, VALUE source, VALUE target);
VALUE cIGraph_adhesion(VALUE self);
VALUE cIGraph_cohesion(VALUE self);

//Community
VALUE cIGraph_modularity                         (VALUE self, VALUE groups);
VALUE cIGraph_community_to_membership            (VALUE self, VALUE merge, 
						  VALUE steps, VALUE nodes);
VALUE cIGraph_community_spinglass                (VALUE self, VALUE weights, 
						  VALUE spins, 
						  VALUE parupdate,
						  VALUE starttemp, 
						  VALUE stoptemp, 
						  VALUE coolfact, 
						  VALUE update_rule, 
						  VALUE gamma);
VALUE cIGraph_community_spinglass_single         (VALUE self, VALUE weights, 
						  VALUE vertex, 
						  VALUE spins, 
						  VALUE update_rule, 
						  VALUE gamma);
VALUE cIGraph_community_leading_eigenvector      (VALUE self, VALUE steps);
VALUE cIGraph_community_leading_eigenvector_naive(VALUE self, VALUE steps);
VALUE cIGraph_community_leading_eigenvector_step (VALUE self, 
						  VALUE membership, 
						  VALUE steps);
VALUE cIGraph_community_walktrap                 (VALUE self, 
						  VALUE weights, 
						  VALUE steps);
VALUE cIGraph_community_edge_betweenness         (VALUE self, 
						  VALUE directed);
VALUE cIGraph_community_eb_get_merges            (VALUE self, 
						  VALUE edges);
VALUE cIGraph_community_fastgreedy               (VALUE self,VALUE weights);
VALUE cIGraph_community_label_propagation        (VALUE self,VALUE weights);

//Attributes
int cIGraph_attribute_init(igraph_t *graph, 
			   igraph_vector_ptr_t *attr);
void cIGraph_attribute_destroy(igraph_t *graph);
int cIGraph_attribute_copy(igraph_t *to, 
			   const igraph_t *from);
int cIGraph_attribute_add_vertices(igraph_t *graph, 
				   long int nv, 
				   igraph_vector_ptr_t *attr);
void cIGraph_attribute_delete_edges(igraph_t *graph, 
				    const igraph_vector_t *idx);
void cIGraph_attribute_delete_vertices(igraph_t *graph,
				       const igraph_vector_t *eidx,
				       const igraph_vector_t *vidx);
int cIGraph_attribute_add_edges(igraph_t *graph, 
				const igraph_vector_t *edges, 
				igraph_vector_ptr_t *attr);
void cIGraph_attribute_delete_edges(igraph_t *graph, 
				    const igraph_vector_t *idx);
int cIGraph_attribute_permute_edges(igraph_t *graph,
				    const igraph_vector_t *idx);
int cIGraph_attribute_get_info(const igraph_t *graph,
			       igraph_strvector_t *gnames,
			       igraph_vector_t *gtypes,
			       igraph_strvector_t *vnames,
			       igraph_vector_t *vtypes,
			       igraph_strvector_t *enames,
			       igraph_vector_t *etypes);
igraph_bool_t cIGraph_attribute_has_attr(const igraph_t *graph,
					 igraph_attribute_elemtype_t type,
					 const char* name);
int cIGraph_attribute_get_type(const igraph_t *graph,
			       igraph_attribute_type_t *type,
			       igraph_attribute_elemtype_t elemtype,
			       const char *name);
int cIGraph_get_numeric_graph_attr(const igraph_t *graph,
				   const char *name, 
				   igraph_vector_t *value);
int cIGraph_get_string_graph_attr(const igraph_t *graph,
				  const char *name, 
				  igraph_strvector_t *value);
int cIGraph_get_numeric_vertex_attr(const igraph_t *graph,
				    const char *name,
				    igraph_vs_t vs,
				    igraph_vector_t *value);
int cIGraph_get_string_vertex_attr(const igraph_t *graph,
				   const char *name,
				   igraph_vs_t vs,
				   igraph_strvector_t *value);
int cIGraph_get_numeric_edge_attr(const igraph_t *graph,
				  const char *name,
				  igraph_es_t es,
				  igraph_vector_t *value);
int cIGraph_get_string_edge_attr(const igraph_t *graph,
				 const char *name,
				 igraph_es_t es,
				 igraph_strvector_t *value);


//Matrix functions
void cIGraph_matrix_free(void *p);
VALUE cIGraph_matrix_alloc(VALUE klass);
VALUE cIGraph_matrix_init_copy(VALUE copy, VALUE orig);
VALUE cIGraph_matrix_initialize(int argc, VALUE *argv, VALUE self);
void Init_igraphmatrix();

VALUE cIGraph_matrix_each(VALUE self);

VALUE cIGraph_matrix_get (VALUE self, VALUE i, VALUE j);
VALUE cIGraph_matrix_set (VALUE self, VALUE i, VALUE j, VALUE x);
VALUE cIGraph_matrix_size(VALUE self);
VALUE cIGraph_matrix_nrow(VALUE self);
VALUE cIGraph_matrix_ncol(VALUE self);
VALUE cIGraph_matrix_max (VALUE self);

VALUE cIGraph_matrix_scale(VALUE self, VALUE x);

VALUE cIGraph_matrix_toa(VALUE self);

//Not implemented yet
//VALUE cIGraph_add_rows(VALUE self, VALUE n);
//VALUE cIGraph_add_cols(VALUE self, VALUE n);
//VALUE cIGraph_matrix_resize(VALUE self, VALUE nrow, VALUE ncol);
