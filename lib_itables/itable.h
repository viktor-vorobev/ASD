#ifndef ITABLE_H
#define ITABLE_H

#include <string>
#include <vector>
#include <stdexcept>

template <class TKey, class TVal>
class ITable {
public:
    virtual ~ITable() = default;

    // Основные операции
    virtual void insert(const TKey& key, const TVal& value) = 0;
    virtual void remove(const TKey& key) = 0;
    virtual TVal find(const TKey& key) const = 0;
    virtual bool contains(const TKey& key) const = 0;

    // Операции для работы с таблицей
    virtual void clear() = 0;
    virtual bool isEmpty() const = 0;
    virtual int size() const = 0;

    // Получение всех ключей/значений
    virtual std::vector<TKey> getKeys() const = 0;
    virtual std::vector<TVal> getValues() const = 0;

    // Оператор доступа
    virtual TVal& operator[](const TKey& key) = 0;
    virtual const TVal& operator[](const TKey& key) const = 0;

    // Сохранение/загрузка
    virtual std::string serialize() const = 0;
    virtual void deserialize(const std::string& data) = 0;
};

// Исключение для таблиц
class TableException : public std::runtime_error {
public:
    explicit TableException(const std::string& message);
};

#endif // ITABLE_H