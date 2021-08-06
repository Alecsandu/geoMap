#include "ObjectLoader.hpp"

ObjectLoader::ObjectLoader()
{
	m_out_vertices = std::vector<glm::vec3>();
	m_out_uvs = std::vector<glm::vec2>();
	m_out_normals = std::vector<glm::vec3>();
}

ObjectLoader::ObjectLoader(const char* object_path)
{
	bool loading_result = this->try_load_object(object_path);
	if (loading_result)
		std::cout << "Fisier incarcat!\n";
	else
		std::cout << "Fisierul dat prin calea: " << object_path << " nu a fost gasit\n";
}

ObjectLoader::~ObjectLoader()
{
}

/* getters for vertices, UVs and normals */
std::vector<glm::vec3> ObjectLoader::getVertices()
{
	return this->m_out_vertices;
}

std::vector<glm::vec2> ObjectLoader::getUVs()
{
	return this->m_out_uvs;
}

std::vector<glm::vec3> ObjectLoader::getNormals()
{
	return this->m_out_normals;
}

/* setters for vertices, UVs and normals */
void ObjectLoader::setVertices(std::vector<glm::vec3> new_vertices)
{
	this->m_out_vertices = new_vertices;
}

void ObjectLoader::setUVs(std::vector<glm::vec2> new_uvs)
{
	this->m_out_uvs = new_uvs;
}

void ObjectLoader::setNormals(std::vector<glm::vec3> new_normals)
{
	this->m_out_normals = new_normals;
}

void ObjectLoader::cleanup()
{
	m_out_vertices.clear();
	m_out_vertices.shrink_to_fit();
	m_out_uvs.clear();
	m_out_uvs.shrink_to_fit();
	m_out_normals.clear();
	m_out_normals.shrink_to_fit();
}

bool ObjectLoader::try_load_object(const char* ruta_fisier)
{
	std::vector<unsigned int> indici_varfuri, indici_coord_texturi, indici_normale;
	std::vector<glm::vec3> varfuri_temporare;
	std::vector<glm::vec2> coordonate_texturi_temporare;
	std::vector<glm::vec3> normale_temporare;

	FILE* f_in = fopen(ruta_fisier, "r");
	if (f_in == NULL)
	{
		std::cout << "Fisierul dat nu poate fi deschis!\n";
		return false;
	}

	while (true)
	{
		char currentLine[32] = { 0 };
		int res = fscanf(f_in, "%31s", currentLine);
		if (res == EOF)
			break;

		if (strcmp(currentLine, "v") == 0)
		{
			glm::vec3 vertex;
			int nr = fscanf(f_in, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			varfuri_temporare.push_back(vertex);
		}
		else if (strcmp(currentLine, "vt") == 0)
		{
			glm::vec2 uv;
			int nr = fscanf(f_in, "%f %f\n", &uv.x, &uv.y);
			uv.y = -uv.y;
			coordonate_texturi_temporare.push_back(uv);
		}
		else if (strcmp(currentLine, "vn") == 0)
		{
			glm::vec3 normal;
			int nr = fscanf(f_in, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			normale_temporare.push_back(normal);
		}
		else if (strcmp(currentLine, "f") == 0)
		{
			std::string vertex1, vertex2, vertex3;
			unsigned int vertex_index[3], uv_index[3], normal_index[3];
			int number_of_elements = fscanf(f_in, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertex_index[0], &uv_index[0], &normal_index[0], &vertex_index[1], &uv_index[1], &normal_index[1], &vertex_index[2], &uv_index[2], &normal_index[2]);
			if (number_of_elements != 9)
			{
				std::cout << "Structura fisierului este ambigua! Verificati fisierele .obj pentru a cauta problema.";
				fclose(f_in);
				return false;
			}
			indici_varfuri.push_back(vertex_index[0]);
			indici_varfuri.push_back(vertex_index[1]);
			indici_varfuri.push_back(vertex_index[2]);
			indici_coord_texturi.push_back(uv_index[0]);
			indici_coord_texturi.push_back(uv_index[1]);
			indici_coord_texturi.push_back(uv_index[2]);
			indici_normale.push_back(normal_index[0]);
			indici_normale.push_back(normal_index[1]);
			indici_normale.push_back(normal_index[2]);
		}
		else
		{
			char random_characters[1000] = { 0 };
			int insignificant = fscanf(f_in, "%999\n", random_characters);
		}
	}

	// Pentru fiecare varf al fiecarui triunghi
	for (unsigned int i = 0; i < indici_varfuri.size(); ++i)
	{
		// iau indicele
		unsigned int indice_varf = indici_varfuri[i];
		unsigned int indice_coordonata_tex = indici_coord_texturi[i];
		unsigned int indice_normala = indici_normale[i];

		// salvez datele de la indice – 1 pentru că sunt stocate de la 0
		glm::vec3 varf = varfuri_temporare[indice_varf - 1];
		glm::vec2 coord_tex = coordonate_texturi_temporare[indice_coordonata_tex - 1];
		glm::vec3 normala = normale_temporare[indice_normala - 1];

		// salvez datele obținute
		m_out_vertices.push_back(varf);
		m_out_uvs.push_back(coord_tex);
		m_out_normals.push_back(normala);
	}

	fclose(f_in);
	return true;
}
