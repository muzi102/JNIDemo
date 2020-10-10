package com.example.jnidemo;

import android.util.Log;

/**
 * @author :muzi102
 * @time : 2020/8/26
 * @Description
 */
public class Student {
    public String name;
    private int age = 0;

    public Student(String name, int age) {
        this.name = name;
        this.age = age;
        Log.e("muzi", toString());
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getAge() {
        return age;
    }

    public void setAge(int age) {
        this.age = age;
    }

    @Override
    public String toString() {
        return "Student{" +
                "name='" + name + '\'' +
                ", age=" + age +
                '}';
    }
}


