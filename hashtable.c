#include "hashtable.h"
#include "small_string.h"

HashTable* MakeTable(const uint16_t size, const float max_load) {
    HashTable* table = (HashTable*) malloc(sizeof(HashTable));
    table->table = (Node*) malloc(sizeof(Node) * size);
    table->size = size;
    table->max_load = max_load;
    table->n_nodes = 0;
    Node empty;
    empty.key = NOTHING;
    empty.value = NOTHING;
    for (uint16_t i = 0; i < size; i++) {
        table->table[i] = empty;
    }
    return table;
}

HashMap* MakeMap(uint16_t size, float max_load) {
    HashMap* map = (HashMap*) malloc(sizeof(HashMap));
    map->map = (Entry*) malloc(sizeof(Entry) * size);
    map->size = size;
    map->max_load = max_load;
    map->n_entries = 0;
    Entry empty;
    JsonElement eKey;
    eKey.json = NULL;
    JsonElement eEl;
    eEl.json = NULL;
    empty.key = eKey;
    empty.el = eEl;
    for (uint16_t i = 0; i < size; i++) {
        map->map[i] = empty;
    }
    return map;
}

void DestroyTable(HashTable* const table) {
    free(table->table);
    free(table);
}

void DestroyMap(HashMap* map) {
    free(map->map);
    free(map);
}

void _TableRehash(HashTable* const table) {
    HashTable* n_table = MakeTable((const uint16_t) (table->size * 2), table->max_load);
    for (uint16_t i = 0; i < table->size; i++) {
        Node node = table->table[i];
        if (node.value != NOTHING) {
            TablePut(n_table, node.key, node.value);
        }
    }
    free(table->table);
    table->table = n_table->table;
    table->size = n_table->size;
    free(n_table);
}

void _MapRehash(HashMap* map) {
    HashMap* n_map = MakeMap((const uint16_t) (map->size * 2), map->max_load);
    for (uint16_t i = 0; i < map->size; i++) {
        Entry entry = map->map[i];
        if (entry.key.json != NULL) {
            MapPut(n_map, entry.key, entry.el);
        }
    }
    free(map->map);
    map->map = n_map->map;
    map->size = n_map->size;
    free(n_map);
}

void TablePut(HashTable* const table, const uint16_t key, const uint16_t value) {
    uint16_t hash = _Hash(key) % table->size;
    while (table->table[hash].value != NOTHING) {
        hash++;
        if (hash >= table->size) {
            hash = 0;
        }
    }
    Node node;
    node.key = key;
    node.value = value;
    table->n_nodes++;
    table->table[hash] = node;
    if (table->n_nodes > table->max_load * table->size) {
        _TableRehash(table);
    }
}

void MapPut(HashMap* map, JsonElement key, JsonElement el) {
    uint16_t hash = (uint16_t) (_MapHash(key) % map->size);
    while (map->map[hash].key.json != NULL) {
        hash++;
        if (hash >= map->size) {
            hash = 0;
        }
    }
    Entry entry;
    entry.key = key;
    entry.el = el;
    map->n_entries++;
    map->map[hash] = entry;
    if (map->n_entries > map->max_load * map->size) {
        _MapRehash(map);
    }
}

uint16_t TableGet(const HashTable* const table, const uint16_t key) {
    uint16_t hash = _Hash(key) % table->size;
    while (table->table[hash].key != key) {
        hash++;
        if (hash >= table->size) {
            hash = 0;
        }
    }
    return table->table[hash].value;
}

JsonElement MapGet(const HashMap* map, JsonElement key) {
    uint16_t hash = (uint16_t) (_MapHash(key) % map->size);
    while (!SegmentsEqual(
            map->map[hash].key.json,
            key.json,
            map->map[hash].key.start,
            map->map[hash].key.end,
            key.start,
            key.end
    )) {
        hash++;
        if (hash >= map->size) {
            hash = 0;
        }
    }
    return map->map[hash].el;
}

uint16_t _Hash(uint16_t i) {
    unsigned int x = i;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return (uint16_t) x;
}

uint32_t _MapHash(JsonElement key) {
    uint32_t h = 0;
    if (key.start == key.end) {
        return h;
    }
    for (uint16_t i = key.start; i < key.end; i++) {
        h = (h << 7) - h + SmallStrCharAt(i, key.json);
    }
    return h;
}

uint16_t MapLength(HashMap* map) {
    uint16_t len = 0;
    for (uint16_t i = 0; i < map->size; i++) {
        if (map->map[i].key.json != NULL) {
            len++;
        }
    }
    return len;
}
