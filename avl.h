#ifndef AVL_H
#define AVL_H

#include <iostream>

using namespace std;

template <typename T>
class AVL {
private:
    struct NodeAVL {
        T valor;
        int altura;
        NodeAVL* izq;
        NodeAVL* der;

        NodeAVL(T v) : valor(v), altura(0), izq(nullptr), der(nullptr) {}
    };

    NodeAVL* root;

    int Altura(NodeAVL* nod) {
        if (!nod) return -1;
        return nod->altura;
    }

    int FB(NodeAVL* nod) {
        if (!nod) return 0;
        return Altura(nod->izq) - Altura(nod->der);
    }

    void Actualizar_altura(NodeAVL* nod) {
        if (nod != nullptr) {
            int alt_izq = Altura(nod->izq);
            int alt_der = Altura(nod->der);
            nod->altura = 1 + (alt_izq > alt_der ? alt_izq : alt_der);
        }
    }

    NodeAVL* RR(NodeAVL* nod) {
        if (!nod || !nod->der) return nod;
        NodeAVL* aux = nod->der;
        nod->der = aux->izq;
        aux->izq = nod;
        Actualizar_altura(nod);
        Actualizar_altura(aux);
        return aux;
    }

    NodeAVL* LL(NodeAVL* nod) {
        if (!nod || !nod->izq) return nod;
        NodeAVL* aux = nod->izq;
        nod->izq = aux->der;
        aux->der = nod;
        Actualizar_altura(nod);
        Actualizar_altura(aux);
        return aux;
    }

    NodeAVL* RL(NodeAVL* nod) {
        if (!nod || !nod->der) return nod;
        nod->der = LL(nod->der);
        return RR(nod);
    }

    NodeAVL* LR(NodeAVL* nod) {
        if (!nod || !nod->izq) return nod;
        nod->izq = RR(nod->izq);
        return LL(nod);
    }

    NodeAVL* Rebalancear(NodeAVL* nod) {
        if (!nod) return nullptr;
        int bal = FB(nod);
        if (bal > 1) {
            if (FB(nod->izq) < 0) {
                return LR(nod);
            }
            return LL(nod);
        }
        if (bal < -1) {
            if (FB(nod->der) > 0) {
                return RL(nod);
            }
            return RR(nod);
        }
        return nod;
    }

    NodeAVL* Sucesor(NodeAVL* nod) {
        if (!nod || !nod->der) return nullptr;
        NodeAVL* act = nod->der;
        while (act->izq != nullptr) {
            act = act->izq;
        }
        return act;
    }

    NodeAVL* Insertar(NodeAVL* nod, T val) {
        if (nod == nullptr) {
            return new NodeAVL(val);
        }
        if (val < nod->valor) {
            nod->izq = Insertar(nod->izq, val);
        } else if (val > nod->valor) {
            nod->der = Insertar(nod->der, val);
        } else {
            return nod;
        }
        Actualizar_altura(nod);
        return Rebalancear(nod);
    }

    NodeAVL* Eliminar(NodeAVL* nod, T val) {
        if (nod == nullptr) {
            return nullptr;
        }
        if (val < nod->valor) {
            nod->izq = Eliminar(nod->izq, val);
        } else if (val > nod->valor) {
            nod->der = Eliminar(nod->der, val);
        } else {
            if (nod->izq == nullptr) {
                NodeAVL* aux = nod->der;
                delete nod;
                return aux;
            } else if (nod->der == nullptr) {
                NodeAVL* aux = nod->izq;
                delete nod;
                return aux;
            } else {
                NodeAVL* suc = Sucesor(nod);
                nod->valor = suc->valor;
                nod->der = Eliminar(nod->der, suc->valor);
            }
        }
        Actualizar_altura(nod);
        return Rebalancear(nod);
    }

    NodeAVL* Buscar_Node(NodeAVL* nod, T val) {
        NodeAVL* act = nod;
        while (act != nullptr) {
            if (val == act->valor) {
                return act;
            }
            if (val < act->valor) {
                act = act->izq;
            } else {
                act = act->der;
            }
        }
        return nullptr;
    }

    bool Existe_Nodo(NodeAVL* nod, T val) {
        return Buscar_Node(nod, val) != nullptr;
    }

    void Liberar(NodeAVL* nod) {
        if (nod != nullptr) {
            Liberar(nod->izq);
            Liberar(nod->der);
            delete nod;
        }
    }

public:
    AVL() {
        root = nullptr;
    }

    ~AVL() {
        Liberar(root);
    }

    void Insertar(T val) {
        root = Insertar(root, val);
    }

    void Eliminar(T val) {
        root = Eliminar(root, val);
    }

    bool Existe_Nodo(T val) {
        return Existe_Nodo(root, val);
    }

    NodeAVL* Buscar(T val) {
        return Buscar_Node(root, val);
    }
};

#endif
