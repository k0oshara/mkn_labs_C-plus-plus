#include "shared_ptr.hpp"
#include <iostream>

void shared_ptr::Storage::incr()
{
    ref_count_++;
}

void shared_ptr::Storage::decr()
{
    ref_count_--;
}

int shared_ptr::Storage::getCounter() const
{
    return ref_count_;
}

Matrix *shared_ptr::Storage::getObject() const
{
    return data_;
}

shared_ptr::Storage::Storage(Matrix *mtx) : data_(mtx), ref_count_(0) {}

shared_ptr::Storage::~Storage()
{
    delete data_;
}

shared_ptr::shared_ptr(Matrix *obj)
{
    if (obj != nullptr)
    {
        storage_ = new Storage(obj);
        storage_->incr();
    }
    else
        storage_ = nullptr;
}

shared_ptr::~shared_ptr()
{

    if (!isNull()) {
        storage_->decr();
        if (storage_->getCounter() == 0) {
            delete storage_;
        }
    }
}

shared_ptr::shared_ptr(const shared_ptr &other) : storage_(other.storage_)
{
    if (!isNull())
        storage_->incr();
}

void shared_ptr::storage_delete()
{
    if (!isNull()) {
        storage_->decr();
        if (storage_->getCounter() == 0) {
            delete storage_;
        }
    }
}

shared_ptr &shared_ptr::operator=(const shared_ptr& other)
{
    if (this != &other) {
        storage_delete();
        storage_ = other.storage_;
        storage_->incr();
    }
    return *this;
}

Matrix* shared_ptr::ptr() const
{
    if (!isNull())
        return storage_->getObject();

    else
        return nullptr;

}

Matrix *shared_ptr::operator->() const
{
    return storage_->getObject();
}

Matrix &shared_ptr::operator*() const
{
    return *storage_->getObject();
}

bool shared_ptr::isNull() const
{
    return storage_ == nullptr;
}

void shared_ptr::reset(Matrix *obj)
{
    storage_delete();
    if (obj != nullptr) {
        storage_ = new Storage(obj);
        storage_->incr();
    }
    else {
        storage_ = nullptr;
    }
}
