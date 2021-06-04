-- phpMyAdmin SQL Dump
-- version 4.8.2
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Mar 10, 2019 at 06:09 PM
-- Server version: 10.1.26-MariaDB
-- PHP Version: 7.1.9

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `vahicle fuel monitoring`
--

-- --------------------------------------------------------

--
-- Table structure for table `fuel_info`
--

CREATE TABLE `fuel_info` (
  `per_unit_price` varchar(100) NOT NULL,
  `fuel_type` varchar(100) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `statement`
--

CREATE TABLE `statement` (
  `balance` mediumtext NOT NULL,
  `p_balance` mediumtext NOT NULL,
  `cost` mediumtext NOT NULL,
  `phone_no` varchar(11) NOT NULL,
  `fuel_type` varchar(50) NOT NULL,
  `fuel_qnt` varchar(50) NOT NULL,
  `date_time` varchar(50) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `statement`
--

INSERT INTO `statement` (`balance`, `p_balance`, `cost`, `phone_no`, `fuel_type`, `fuel_qnt`, `date_time`) VALUES
('4000', '4500', '500', '01680476738', 'Petrol', '5.10', '2018-12-27 12:25:46'),
('3000', '4000', '1000', '01680476738', 'Petrol', '10.20', '2018-12-27 16:50:46');

-- --------------------------------------------------------

--
-- Table structure for table `user`
--

CREATE TABLE `user` (
  `phone_no` varchar(11) NOT NULL,
  `name` varchar(50) NOT NULL,
  `address` varchar(100) NOT NULL,
  `email` varchar(50) NOT NULL,
  `age` varchar(5) NOT NULL,
  `gender` varchar(10) NOT NULL,
  `rfid_no` varchar(500) DEFAULT NULL,
  `pass` varchar(20) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `user`
--

INSERT INTO `user` (`phone_no`, `name`, `address`, `email`, `age`, `gender`, `rfid_no`, `pass`) VALUES
('01680476738', 'Mahid Shawon', 'Dhaka', 'mahidur41@gmail.com', '23', 'Male', '94E73AF6', '123456789');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `user`
--
ALTER TABLE `user`
  ADD PRIMARY KEY (`phone_no`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
