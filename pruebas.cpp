#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>

class BTreeNode {
    int *keys;    // Arreglo de claves
    int t;        // Grado mínimo
    BTreeNode **children; // Arreglo de punteros a los hijos
    int n;        // Número actual de claves
    bool isLeaf;  // Es verdadero si el nodo es una hoja

public:
    BTreeNode(int t, bool isLeaf);

    void insertNonFull(int key);

    void splitChild(int i, BTreeNode *y);

    void traverse();

    void graficar();

    void graficarRecursivo(std::ofstream& out) const;

    BTreeNode *search(int key);

    friend class BTree;
};

class BTree {
    BTreeNode *root;
    int t; // Grado mínimo

public:
    BTree(int _t) {
        root = nullptr;
        t = _t;
    }

    void traverse() {
        if (root != nullptr)
            root->traverse();
    }

    BTreeNode* search(int key) {
        return (root == nullptr) ? nullptr : root->search(key);
    }

    void insert(int key);
    void graficar() const;
};

BTreeNode::BTreeNode(int t1, bool leaf) {
    t = t1;
    isLeaf = leaf;

    keys = new int[2 * t - 1];
    children = new BTreeNode *[2 * t];

    n = 0;
}

void BTreeNode::traverse() {
    int i;
    for (i = 0; i < n; i++) {
        if (!isLeaf)
            children[i]->traverse();
        std::cout << " " << keys[i];
    }

    if (!isLeaf)
        children[i]->traverse();
}

BTreeNode* BTreeNode::search(int key) {
    int i = 0;
    while (i < n && key > keys[i])
        i++;

    if (i < n && keys[i] == key)
        return this;

    if (isLeaf)
        return nullptr;

    return children[i]->search(key);
}

void BTree::insert(int key) {
    if (root == nullptr) {
        root = new BTreeNode(t, true);
        root->keys[0] = key;
        root->n = 1;
    } else {
        if (root->n == 2 * t - 1) {
            BTreeNode *s = new BTreeNode(t, false);

            s->children[0] = root;

            s->splitChild(0, root);

            int i = 0;
            if (s->keys[0] < key)
                i++;
            s->children[i]->insertNonFull(key);

            root = s;
        } else
            root->insertNonFull(key);
    }
}

void BTreeNode::insertNonFull(int k) {
    int i = n - 1;

    if (isLeaf) {
        // Insertar la nueva llave en el nodo hoja correspondiente
        while (i >= 0 && keys[i] > k) {
            keys[i + 1] = keys[i];
            i--;
        }
        keys[i + 1] = k;
        n++;
    } else {
        // Encontrar el hijo que va a tener la nueva llave
        while (i >= 0 && keys[i] > k) i--;
        // Ver si el hijo encontrado está lleno
        if (children[i + 1]->n == 2*t - 1) {
            // Dividir el hijo si está lleno
            splitChild(i + 1, children[i + 1]);

            // Después de dividir, la llave del medio de children[i] sube y
            // children[i] se divide en dos. Ver cuál de los dos va a tener la nueva llave
            if (keys[i + 1] < k) i++;
        }
        children[i + 1]->insertNonFull(k);
    }
}

void BTreeNode::splitChild(int i, BTreeNode *y) {
    // Crear un nuevo nodo que va a almacenar (t-1) llaves de y
    BTreeNode *z = new BTreeNode(y->t, y->isLeaf);
    z->n = t - 1; // t es 3 para un árbol B de grado máximo 5

    // Copiar las últimas (t-1) llaves de y a z
    for (int j = 0; j < t - 1; j++)
        z->keys[j] = y->keys[j + t];

    // Copiar los últimos t hijos de y a z si no es una hoja
    if (!y->isLeaf) {
        for (int j = 0; j < t; j++)
            z->children[j] = y->children[j + t];
    }

    y->n = t - 1; // Reducir el número de llaves en y

    // Mover los hijos del nodo actual para hacer espacio para el nuevo hijo
    for (int j = n; j >= i + 1; j--)
        children[j + 1] = children[j];

    // Enlazar el nuevo hijo al nodo actual
    children[i + 1] = z;

    // Mover una llave de y al nodo actual
    for (int j = n - 1; j >= i; j--)
        keys[j + 1] = keys[j];

    keys[i] = y->keys[t - 1];

    // Incrementar el número de llaves en este nodo
    n++;
}

void BTreeNode::graficarRecursivo(std::ofstream& out) const {
    if (this == nullptr) return;

    // Escribir la descripción de este nodo
    out << "    \"" << this << "\" [label=\"";
    for (int i = 0; i < n; ++i) {
        out << keys[i];
        if (i < n - 1) out << "|";
    }
    out << "\"];\n";

    // Si no es una hoja, escribir las conexiones con los nodos hijos y llamar recursivamente
    if (!isLeaf) {
        for (int i = 0; i <= n; ++i) {
            out << "    \"" << this << "\" -> \"" << children[i] << "\";\n";
            children[i]->graficarRecursivo(out);
        }
    }
}

void BTree::graficar() const {
    std::ofstream out("arbolB.dot");
    out << "digraph ArbolB {\n";
    out << "    node [shape = record,height=.1];\n";
    if (root != nullptr) {
        root->graficarRecursivo(out);
    }
    out << "}\n";
    out.close();

    // Llamar a Graphviz para generar la imagen y luego abrirla
    system("dot -Tpng arbolB.dot -o arbolB.png");
    system("arbolB.png");
}

int main() {
    BTree t(3); // Grado mínimo 3 (orden 5 significa grado mínimo 3)

    t.insert(10);
    t.insert(20);
    t.insert(5);
    t.insert(6);
    t.insert(12);
    

    std::cout << "Recorrido del árbol B es: ";
    t.traverse();

    int key = 6;
    (t.search(key) != nullptr) ? std::cout << "\nClave " << key << " presente\n" : std::cout << "\nClave " << key << " no presente\n";

    key = 15;
    (t.search(key) != nullptr) ? std::cout << "Clave " << key << " presente\n" : std::cout << "Clave " << key << " no presente\n";

    t.graficar();

    return 0;
}